#include <iostream>
#include <string>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/program_options.hpp>

#include "embedder/consts.h"
#include "image/image_matrix.h"
#include "image/bmp_image.h"
#include "options.h"
#include "logging.h"

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    /* Initialize constants */
    rdh::Consts::Instance();

    po::variables_map vm;
    po::options_description desc{ "Options" };
    desc.add_options()
        ("help,h", "Prints help message.")
        ("image-path", po::value<std::string>()->required(), "Path to an image to work with.")
        ("second-image", po::value<std::string>(), "Path to second image (only used in calculate PSNR/SSIM).")
        ("result-path", po::value<std::string>(), "Path to save decrypted/encrypted image or image with embedded data.")
        ("result-path-data", po::value<std::string>(), "Path to save extracted data from image to.")
        ("mode", po::value<std::string>()->required(), "Specifies execution mode.\n"
            "Can be one of the follows:\n"
            "  show: \tDisplay image specified in --image-path.\n"
            "  encrypt: \tEncrypts image specified in --image-path using key provided in --encryption-key. "
            "Result will be saved in --result-path.\n"
            "  embed  : \tEmbeds additional data specified in --data-file inside image specified in --image-path using key provided in --embed-key. "
            "Result will be saved in --result-path.\n"
            "  decrypt: \tDecrypts image specified in --image-path using key provided in --encryption-key. "
            "Result will be saved in --result-path.\n"
            "  extract: \tExtracts data or recovers image or both, from image specified in --image-path using keys provided in --embed-key and --encryption-key. "
            "Result will be saved in --result-path/--result-path-data.\n"
            "    Example-1 \t(image recovery mode): ./rdh.exe --mode extract --image-path ./marked-encrypted.bmp --result-path ./extracted.bmp --encryption-key AABBCC\n"
            "    Example-2 \t(image recovery and data extraction mode): ./rdh.exe --mode extract --image-path ./marked-encrypted.bmp --result-path ./extracted.bmp --result-path-data ./extracted.bin --encryption-key AABBCC --embed-key FFDDEE\n"
            "    Example-3 \t(data extraction mode): ./rdh.exe --mode extract --image-path ./marked-encrypted.bmp --result-path-data ./extracted.bin --embed-key FFDDEE\n"
            "  calculate-psnr: \tCalculates PSNR for images specified in --image-path and --second-image. "
            "Result will be printed to the console.\n"
            "  calculate-ssim: \tCalculates SSIM for images specified in --image-path and --second-image. "
            "Result will be printed to the console.\n")
        ("encryption-key", po::value<std::string>(), "Image encryption/decryption key.\n"
            "  Example: --encryption-key AA00BB11CC22DD33EE\n")
        ("enc-key-file", po::value<std::string>(), "Image encryption/decryption key file (key should be in binary form). (can be used instead of encryption-key)\n"
            "  Example: --enc-key-file ./enc_key_file.bin\n")
        ("embed-key", po::value<std::string>(), "Data embedding/extraction key.\n"
            "  Example: --embed-key AA00BB11CC22DD33EE\n")
        ("embed-key-file", po::value<std::string>(), "Data embedding/extraction key (key should be in binary form). (can be used instead of data-key)\n"
            "  Example: --embed-key-file ./embed_key_file.bin\n")
        ("data-file", po::value<std::string>(), "Path to file with additional data to embed in the encrypted image\n"
            "  Example: --data-file ./additional_data.bin\n")
        ("threshold", po::value<uint16_t>()->default_value(rdh::Consts::Instance().GetThreshold()), "Threshold parameter for blocks classification. Allowed values are: 0, ..., 24.\n"
            "  Example: --threshold 20")
        ("lsb-layers", po::value<uint16_t>()->default_value(rdh::Consts::Instance().GetLsbLayers()), "Lsb layers to use for data embedding. Allowed values are: 0, ..., 7.\n"
            "  Example: --lsb-layers 3")
        ("lambda", po::value<uint16_t>()->default_value(rdh::Consts::Instance().GetLambda()), "Blocks group size.\n"
            "  Example: --lambda 100")
        ("alpha", po::value<uint16_t>()->default_value(rdh::Consts::Instance().GetAlpha()), "Number of bits to embed in each group.\n"
            "  Example: --alpha 5")
        ("lsb-hash-size", po::value<uint16_t>()->default_value(rdh::Consts::Instance().GetLsbHashSize()), "Length of hash for each group.\n"
            "  Example: --lsb-hash-size 3")
        ("log-level", po::value<boost::log::trivial::severity_level>()->default_value(boost::log::trivial::severity_level::fatal), 
            "Log level\n"
            "  Example: --log-level [trace, debug, info, warning, error, fatal]\n");

    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
    }
    catch (const po::unknown_option& ex) {
        std::cout << ex.what() << std::endl;
        std::cout << "Run with --help to read the docs" << std::endl;
        return 1;
    }
    catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;
        std::cout << "Run with --help to read the docs" << std::endl;
        return 1;
    }

    if (vm.size() == 0 || vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    rdh::log::InitLogger(vm["log-level"].as<boost::log::trivial::severity_level>());

    std::string mode;
    std::string imagePath;

    try {
        mode = vm["mode"].as<std::string>();
        imagePath = vm["image-path"].as<std::string>();

        /* Check for allowed parameters intervals. */
        if (vm["threshold"].as<uint16_t>() > 24) {
            std::cout << "Threshold value can not be bigger than 24!" << std::endl;
            std::cout << "Run with --help to read the docs" << std::endl;
            return 1;
        }
        else {
            rdh::Consts::Instance().UpdateThreshold(vm["threshold"].as<uint16_t>());
        }
        
        if (vm["lsb-layers"].as<uint16_t>() > 7) {
            std::cout << "You can't use more than 7 Least Significant Bits for data embedding!" << std::endl;
            std::cout << "Run with --help to read the docs" << std::endl;
            return 1;
        }
        else {
            rdh::Consts::Instance().UpdateLsbLayers(vm["lsb-layers"].as<uint16_t>());
        }

        if (vm["alpha"].as<uint16_t>() <= vm["lsb-hash-size"].as<uint16_t>()) {
            std::cout << "Alpha should be bigger than lsb-hash-size!" << std::endl;
            std::cout << "Run with --help to read the docs" << std::endl;
            return 1;
        }
        else {
            rdh::Consts::Instance().UpdateAlpha(vm["alpha"].as<uint16_t>());
        }

        rdh::Consts::Instance().UpdateLambda(vm["lambda"].as<uint16_t>());
        rdh::Consts::Instance().UpdateLsbHashSize(vm["lsb-hash-size"].as<uint16_t>());
    }
    catch (po::required_option&) {
        std::cout << "Missing one ore more required option!" << std::endl;
        std::cout << "Run with --help to read the docs" << std::endl;
        return 1;
    }
    catch (boost::bad_any_cast&) {
        std::cout << "Missing one ore more required option!" << std::endl;
        std::cout << "Run with --help to read the docs" << std::endl;
        return 1;
    }

    try {
        if (mode == "show") {
            return rdh::Options::HandleShow(imagePath, vm, desc);
        }
        else if (mode == "encrypt") {
            return rdh::Options::HandleEncrypt(imagePath, vm, desc);
        }
        else if (mode == "embed") {
            return rdh::Options::HandleEmbed(imagePath, vm, desc);
        }
        else if (mode == "decrypt") {
            return rdh::Options::HandleDecrypt(imagePath, vm, desc);
        }
        else if (mode == "extract") {
            return rdh::Options::HandleExtractAndRecover(imagePath, vm, desc);
        }
        else if (mode == "calculate-psnr") {
            if (vm.count("second-image") == 0) {
                std::cout << "You must provide result path (--second-image) to the first image." << std::endl;
                std::cout << "Run with --help to read the docs" << std::endl;
                return 1;
            }

            return rdh::Options::HandleCalculatePsnr(imagePath, vm["second-image"].as<std::string>(), vm, desc);
        }
        else if (mode == "calculate-ssim") {
            if (vm.count("second-image") == 0) {
                std::cout << "You must provide result path (--second-image) to the first image." << std::endl;
                std::cout << "Run with --help to read the docs" << std::endl;
                return 1;
            }

            return rdh::Options::HandleCalculateSsim(imagePath, vm["second-image"].as<std::string>(), vm, desc);
        }
        else {
            std::cout << "Incorrect operation mode selected!" << std::endl;
            std::cout << "Run " << argv[0] << " --help, to check available options" << std::endl;
            return 1;
        }
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}

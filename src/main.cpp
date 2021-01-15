#include <iostream>
#include <string>
#include <boost/program_options.hpp>

#include "image_matrix.h"
#include "bmp_image.h"
#include "utils.h"
#include "encryptor.h"

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    po::variables_map vm;
    po::options_description desc{ "Options" };
    desc.add_options()
        ("help,h", "Prints help message.")
        ("image-path", po::value<std::string>()->required(), "Path to an image to work with.")
        ("result-path", po::value<std::string>(), "Path to save decrypted/encrypted image or recovered message to or image with embedded data.")
        ("mode", po::value<std::string>()->required(), "Specifies execution mode.\n"
            "Can be one of the follows:\n"
            "  show: \tDisplay image specified in --image-path.\n"
            "  encrypt: \tEncrypts image specified in --image-path using key provided in --encryption-key. "
            "Result will be saved in --result-path.\n"
            "  hide   : \tHides data in image specified in --image-path using key provided in --data-key. "
            "Result will be saved in --result-path.\n"
            "  decrypt: \tDecrypts image specified in --image-path using key provided in --encryption-key. "
            "Result will be saved in --result-path.\n"
            "  extract: \tExtracts data from image specified in --image-path using key provided in --data-key. "
            "Result will be saved in --result-path.\n"
            "  recover: \tRecovers image specified in --image-path using both keys provided in --data-key and "
            "--encryption-key. Result will be saved in --result-path.\n")
        ("encryption-key", po::value<std::string>(), "Image encryption/decryption key.\n"
            "Example: --encryption-key AA00BB11CC22DD33EE\n")
        ("data-key", po::value<std::string>(), "Data embedding/extraction key.\n"
            "Example: --data-key AA00BB11CC22DD33EE\n");
    
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
    }
    catch (po::unknown_option& e) {
        std::cout << "Unknown option!" << std::endl;
        std::cout << desc << std::endl;
        return 1;
    }

    if (vm.size() == 0 || vm.count("help"))
    {
        std::cout << desc << "\n";
        return 1;
    }

    std::string mode;
    std::string imagePath;

    try {
        mode = vm["mode"].as<std::string>();
        imagePath = vm["image-path"].as<std::string>();
    }
    catch (po::required_option& e) {
        std::cout << "Missing one ore more required option!" << std::endl;
        std::cout << desc << std::endl;
        return 1;
    }
    catch (boost::bad_any_cast&) {
        std::cout << "Missing one ore more required option!" << std::endl;
        std::cout << desc << std::endl;
        return 1;
    }
    
    try {
        if (mode == "show") {
            rdh::BmpImage image(imagePath);
            image.Show();
        }
        else if (mode == "encrypt") {
            if (vm.count("encryption-key") == 0) {
                std::cout << "You must provide encryption key in form of a hex string: \"AABBCC001122\"" << std::endl;
                std::cout << desc << std::endl;
                return 1;
            }

            if (vm.count("result-path") == 0) {
                std::cout << "You must provide result path, to write encrypted image to." << std::endl;
                std::cout << desc << std::endl;
                return 1;
            }

            std::vector<uint8_t> encryptionKey = rdh::utils::HexToBytes<uint8_t>(vm["encryption-key"].as<std::string>());
            rdh::BmpImage image(imagePath);

            if (encryptionKey.size() < static_cast<std::size_t>(image.GetWidth()) * static_cast<std::size_t>(image.GetHeight())) {
                std::cout << "Waring! Encryption key length is less than image size!" << std::endl;
            }

            rdh::Encryptor::Encrypt(image, encryptionKey).Save(vm["result-path"].as<std::string>());
        }
        else if (mode == "hide") {
            assert(false && "Option is not implemented!");
        }
        else if (mode == "decrypt") {
            if (vm.count("encryption-key") == 0) {
                std::cout << "You must provide decryption key in form of a hex string: \"AABBCC001122\"" << std::endl;
                std::cout << desc << std::endl;
                return 1;
            }

            if (vm.count("result-path") == 0) {
                std::cout << "You must provide result path, to write encrypted image to." << std::endl;
                std::cout << desc << std::endl;
                return 1;
            }

            std::vector<uint8_t> decryptionKey = rdh::utils::HexToBytes<uint8_t>(vm["encryption-key"].as<std::string>());
            rdh::BmpImage image(imagePath);

            rdh::Encryptor::Decrypt(image, decryptionKey).Save(vm["result-path"].as<std::string>());
        }
        else if (mode == "extract") {
            assert(false && "Option is not implemented!");
        }
        else if (mode == "recover") {
            assert(false && "Option is not implemented!");
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

#include "options.h"

#include "utils.h"
#include "encryptor/encryptor.h"
#include "embedder/embedder.h"
#include "image/bmp_image.h"

namespace rdh {

    int Options::HandleShow(const std::string& t_ImagePath, po::variables_map& t_Vm, po::options_description& t_Desc)
    {
        rdh::BmpImage image(t_ImagePath);
        image.Show();

        return 0;
    }

    int Options::HandleEncrypt(const std::string& t_ImagePath, po::variables_map& t_Vm, po::options_description& t_Desc)
    {
        if (t_Vm.count("encryption-key") == 0 && t_Vm.count("enc-key-file") == 0) {
            std::cout << "You must provide encryption key via argument (--encryption-key), or use encryption key file (--enc-key-file)!" << std::endl;
            std::cout << t_Desc << std::endl;
            return 1;
        }

        if (t_Vm.count("result-path") == 0) {
            std::cout << "You must provide result path (--result-path), to write encrypted image to." << std::endl;
            std::cout << t_Desc << std::endl;
            return 1;
        }

        std::vector<uint8_t> encryptionKey;
        rdh::BmpImage image(t_ImagePath);
        
        if (t_Vm.count("enc-key-file")) {
            encryptionKey = utils::LoadFileData<uint8_t>(t_Vm["enc-key-file"].as<std::string>());
        }
        else {
            encryptionKey = rdh::utils::HexToBytes<uint8_t>(t_Vm["encryption-key"].as<std::string>());
        }

        if (encryptionKey.size() < static_cast<std::size_t>(image.GetWidth()) * static_cast<std::size_t>(image.GetHeight()) / 4) {
            std::cout << "Warning! Encryption key length is less than (image.width * image.height) / 4!" << std::endl;
        }

        Encryptor::Encrypt(image, encryptionKey).Save(t_Vm["result-path"].as<std::string>());

        std::cout << "Encrypted image saved to: " << t_Vm["result-path"].as<std::string>() << std::endl;

        return 0;
    }

    int Options::HandleEmbed(const std::string& t_ImagePath, po::variables_map& t_Vm, po::options_description& t_Desc)
    {
        if (t_Vm.count("embed-key") == 0 && t_Vm.count("embed-key-file") == 0) {
            std::cout << "You must provide data embedding key via argument (--embed-key), or use embed key file (--embed-key-file)!" << std::endl;
            std::cout << t_Desc << std::endl;
            return 1;
        }

        if (t_Vm.count("result-path") == 0) {
            std::cout << "You must provide result path (--result-path), to write image with embedded data to." << std::endl;
            std::cout << t_Desc << std::endl;
            return 1;
        }

        if (t_Vm.count("data-file") == 0) {
            std::cout << "You must provide path to file with additional data to embed (--data-file) inside the image." << std::endl;
            std::cout << t_Desc << std::endl;
            return 1;
        }

        std::vector<uint8_t> embedKey;
        std::vector<uint8_t> dataToEmbed = utils::LoadFileData<uint8_t>(t_Vm["data-file"].as<std::string>());
        rdh::BmpImage image(t_ImagePath);

        if (t_Vm.count("embed-key-file")) {
            embedKey = utils::LoadFileData<uint8_t>(t_Vm["embed-key-file"].as<std::string>());
        }
        else {
            embedKey = rdh::utils::HexToBytes<uint8_t>(t_Vm["embed-key"].as<std::string>());
        }

        Embedder::Embed(image, dataToEmbed, embedKey).Save(t_Vm["result-path"].as<std::string>());

        std::cout << "Image with embedded data saved to: " << t_Vm["result-path"].as<std::string>() << std::endl;

        return 0;
    }

    int Options::HandleDecrypt(const std::string& t_ImagePath, po::variables_map& t_Vm, po::options_description& t_Desc)
    {
        if (t_Vm.count("encryption-key") == 0 && t_Vm.count("enc-key-file") == 0) {
            std::cout << "You must provide decryption key via argument (--encryption-key), or use decryption key file (--enc-key-file)!" << std::endl;
            std::cout << t_Desc << std::endl;
            return 1;
        }

        if (t_Vm.count("result-path") == 0) {
            std::cout << "You must provide result path, to write encrypted image to." << std::endl;
            std::cout << t_Desc << std::endl;
            return 1;
        }

        std::vector<uint8_t> decryptionKey;
        rdh::BmpImage image(t_ImagePath);

        if (t_Vm.count("enc-key-file")) {
            decryptionKey = utils::LoadFileData<uint8_t>(t_Vm["enc-key-file"].as<std::string>());
        }
        else {
            decryptionKey = rdh::utils::HexToBytes<uint8_t>(t_Vm["encryption-key"].as<std::string>());
        }

        Encryptor::Decrypt(image, decryptionKey).Save(t_Vm["result-path"].as<std::string>());
     
        std::cout << "Decrypted image saved to: " << t_Vm["result-path"].as<std::string>() << std::endl;

        return 0;
    }

    int Options::HandleExtract(const std::string& t_ImagePath, po::variables_map& t_Vm, po::options_description& t_Desc)
    {
        assert(false && "Option is not implemented!");
        return 0;
    }

    int Options::HandleRecover(const std::string& t_ImagePath, po::variables_map& t_Vm, po::options_description& t_Desc)
    {
        assert(false && "Option is not implemented!");
        return 0;
    }
}
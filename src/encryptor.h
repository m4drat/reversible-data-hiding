#pragma once

#include <vector>

#include "bmp_image.h"

namespace rdh {
    class Encryptor {
    public:
        /**
         * @brief Encrypts image t_EncryptedImage using t_EncryptionKey as a key to simple XOR-based crypto algorithm.
         * @param t_PlainImage Plain image to encrypt
         * @param t_EncryptionKey Encryption key
         * @return Encrypted image
        */
        static BmpImage Encrypt(const BmpImage& t_PlainImage, std::vector<uint8_t>& t_EncryptionKey);

        /**
         * @brief Decrypts image t_EncryptedImage using t_DecryptionKey as a key to simple XOR-based crypto algorithm.
         * @param t_EncryptedImage Encrypted image to decrypt
         * @param t_DecryptionKey Decryption key
         * @return Decrypted image
        */
        static BmpImage Decrypt(const BmpImage& t_EncryptedImage, std::vector<uint8_t>& t_DecryptionKey);
    };
}

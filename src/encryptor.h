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

        /**
         * @brief Given pixel (y, x) return key index to use with this pixel.
         * @param Width of an image
         * @param pixel y coordinate
         * @param pixel x coordinate
         * @return key index at specific image (y, x)
        */
        static uint32_t CalculateKeyIndex(uint32_t t_ImageWidth, uint32_t t_Y, uint32_t t_X);
        
        /**
         * @brief Encrypts part of an image t_PlainImage. Part of encrypted image is written into t_EncryptedImage. 
         * @param t_PlainImage image to encrypt
         * @param t_EncryptedImage resulting encrypted image
         * @param t_EncryptionKey encryptioon key
         * @param t_YStart y pixel coordinate to start encryption from (including)
         * @param t_YEnd y pixel coordinate to encrypt to (excluding)
         * @param t_XStart x pixel coordinate to start encryption from (including)
         * @param t_XEnd x pixel coordinate to encrypt to (excluding)
        */
        static void EncryptorWorker(const BmpImage& t_PlainImage,  BmpImage& t_EncryptedImage, const std::vector<uint8_t>& t_EncryptionKey, uint32_t t_YStart, uint32_t t_YEnd, uint32_t t_XStart, uint32_t t_XEnd);
    };
}

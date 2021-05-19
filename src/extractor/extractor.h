#pragma once

#include "types.h"
#include "image/bmp_image.h"
#include "embedder/consts.h"

#include "Eigen/Dense"

namespace rdh {
    class Extractor {
    public:
        /**
         * @brief Recovers image using image encryption key.
         * @param t_MarkedEncryptedImage Image to recover from.
         * @param t_RecoveredImagePath where to save recovered image.
         * @param t_EncryptionKey Image encryption key.
        */
        static void RecoverImage(
            BmpImage& t_MarkedEncryptedImage,
            const std::string t_RecoveredImagePath,
            std::vector<uint8_t>& t_EncryptionKey
        );
        
        /**
         * @brief Extracts data from t_MarkedEncryptedImage using dataEmbeddingKey.
         * @param t_MarkedEncryptedImage Image to extract data from.
         * @param t_ExtractedDataPath where to save extracted data.
         * @param t_DataEmbeddingKey data embedding key.
        */
        static void ExtractData(
            BmpImage& t_MarkedEncryptedImage,
            const std::string t_ExtractedDataPath,
            std::vector<uint8_t>& t_DataEmbeddingKey
        );

        /**
         * @brief Extracts data and recovers image from t_MarkedEncryptedImage using 
         * both encryption and dataEmbedding keys.
         * @param t_MarkedEncryptedImage Image to extract data and recover original image from
         * @param t_RecoveredImagePath where to save recovered image.
         * @param t_ExtractedDataPath where to save extracted data.
         * @param t_DataEmbeddingKey data embedding key.
         * @param t_EncryptionKey Image encryption key.
        */
        static void RecoverImageAndExract(
            BmpImage& t_MarkedEncryptedImage,
            const std::string t_RecoveredImagePath, 
            const std::string t_ExtractedDataPath,
            std::vector<uint8_t>& t_DataEmbeddingKey,
            std::vector<uint8_t>& t_EncryptionKey
        );
    private:
        /**
         * @brief Extracts all of the bitstreams from marked-encrypted image.
         * @param[in] t_MarkedEncryptedImage Image to extract bitstreams from.
         * @param[in] t_DataEmbeddingKey Key, that was used to embed additional data.
         * @param[out] t_RlcEncodedLengths std::vector<uint16_t> of lengths for rlc-compressed blocks.
         * @param[out] t_RlcEncodedBitStream Bitstream of rlc-compressed blocks.
         * @param[out] t_LsbEncodedBitStream Bitstream of lsb-compressed blocks.
         * @param[out] t_HashesBitStream Bitstream of hashes for lsb-compressed groups.
         * @param[out] t_LsbsBitStream Bitstream of LSBs for each block.
         * @param[out] t_UserDataBitStream Bitstream of user-embedded data.
        */
        static void ExtractBitStreams(
            const BmpImage& t_MarkedEncryptedImage,
            std::vector<uint8_t>& t_DataEmbeddingKey, 
            std::optional<std::reference_wrapper<std::vector<uint16_t>>> t_RlcEncodedLengths, 
            std::optional<std::reference_wrapper<std::string>> t_RlcEncodedBitStream,
            std::optional<std::reference_wrapper<std::string>> t_LsbEncodedBitStream,
            std::optional<std::reference_wrapper<std::string>> t_HashesBitStream,
            std::optional<std::reference_wrapper<std::string>> t_LsbsBitStream,
            std::optional<std::reference_wrapper<std::string>> t_UserDataBitStream
        );
    };
}
#pragma once

#include <vector>

#include "types.h"
#include "image/bmp_image.h"
#include "embedder/encoded_block.h"

namespace rdh {
    class Embedder {
    public:
        /**
         * @brief Embeds data into image t_PlainImage
         * @param t_EncryptedEmptyImage Encrypted image where additional data will be embedded
         * @param t_Data Data to embed
         * @param t_DataEmbeddingKey key to use to embed data
         * @return Encrypted image with embedded data into it
        */
        static BmpImage Embed(const BmpImage& t_EncryptedEmptyImage, const std::vector<uint8_t>& t_Data, const std::vector<uint8_t>& t_DataEmbeddingKey);

        /**
         * @brief Encodes given block using LSB-compression technique.
         * @param t_EncodedBlock block to encode
         * @return EncodedBlock, that was passed by reference
        */
        static EncodedBlock& EncodeBlockLsb(EncodedBlock& t_EncodedBlock);

        /**
         * @brief Packs all additional data into a single BMP image.
         * Additional data is: 
         *  - Binary location map (lsb of the first pixel of each block):
         *   1 - block is encoded using RLC encoding;
         *   0 - block is encoded using LSB encoding.
         *  - User supplied data to embed
         *  - 
        */
        static void PackDataIntoBmp();
    };
}
#pragma once

#include <vector>

#include "types.h"
#include "image/bmp_image.h"

namespace rdh {
    class Embedder {
        /**
         * @brief Embeds data into image t_PlainImage
         * @param t_EncryptedEmptyImage Encrypted image where additional data will be embedded
         * @param t_Data Data to embed
         * @param t_DataEmbeddingKey key to use to embed data
         * @return Encrypted image with embedded data into it
        */
        static BmpImage Embed(const BmpImage& t_EncryptedEmptyImage, std::vector<uint8_t>& t_Data, std::vector<uint8_t>& t_DataEmbeddingKey);
    };
}
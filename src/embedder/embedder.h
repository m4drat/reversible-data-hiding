#pragma once

#include <vector>
#include <bitset>

#include "types.h"
#include "image/bmp_image.h"
#include "embedder/consts.h"

#include "Eigen/Dense"

namespace rdh {
    class Embedder {
    public:
        using Group = Eigen::Matrix<uint8_t, consts::c_Lambda* (4 * consts::c_LsbLayers - 1), 1>;

        /**
         * @brief Embeds data into image t_PlainImage
         * @param t_EncryptedEmptyImage Encrypted image where additional data will be embedded
         * @param t_Data Data to embed
         * @param t_DataEmbeddingKey key to use to embed data
         * @return Encrypted image with embedded data into it (Edited t_EncryptedImage).
        */
        static BmpImage& Embed(BmpImage& t_EncryptedImage, const std::vector<uint8_t>& t_Data, const std::vector<uint8_t>& t_DataEmbeddingKey);
    private:
        /**
         * @brief Fills matrix using data key, and PRNG.
         * @param t_Mat matrix to fill.
         * @param t_DataEmbeddingKey key to use, to setup PRNG.
        */
        static void PreparePseudoRandomMatrix(Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>& t_Mat, const std::vector<uint8_t>& t_DataEmbeddingKey);

        /**
         * @brief Generates hash for group G_i.
         * @param t_CurGroup group to calculate hash for
         * @param t_DataEmbeddingKey data embedding key, which is used to generate PR matrix
         * @return "BitString" that represents hash for the block
        */
        static std::string HashLsbBlock(Group& t_CurGroup, const std::vector<uint8_t>& t_DataEmbeddingKey);
    };
}
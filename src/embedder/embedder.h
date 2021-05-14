#pragma once

#include <vector>
#include <bitset>

#include "types.h"
#include "image/bmp_image.h"
#include "embedder/encoder.h"
#include "embedder/consts.h"

#include "Eigen/Dense"

namespace rdh {
    class Embedder {
    public:
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
         * @brief Represents all lsbs from one block, that is going to be encoded using 
         * LSB-based compression algorithm.
         * In the article it's referred as V_{i_q}.
        */
        struct BlockLsbs {
            BlockLsbs(uint16_t t_Px1bits, uint16_t t_Px2bits, uint16_t t_Px3bits, uint16_t t_Px4bits)
                : px1Lsbs{ t_Px1bits }, px2Lsbs{ t_Px2bits }, px3Lsbs{ t_Px3bits }, px4Lsbs{ t_Px4bits }
            {}
            
            /* LSB for px1 is always zero, because it's used as a location map */
            std::bitset<consts::c_LsbLayers> px1Lsbs;
            std::bitset<consts::c_LsbLayers> px2Lsbs;
            std::bitset<consts::c_LsbLayers> px3Lsbs;
            std::bitset<consts::c_LsbLayers> px4Lsbs;
        };
    };
}
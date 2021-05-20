#pragma once

#include <vector>
#include <bitset>

#include "types.h"
#include "image/bmp_image.h"
#include "embedder/consts.h"
#include "extractor/extractor.h"

#include "Eigen/Dense"

namespace rdh {
    class Embedder {
    public:
        using Group = Eigen::Matrix<uint8_t, Eigen::Dynamic, 1>;

        /**
         * @brief Embeds data into image t_PlainImage
         * @param t_EncryptedEmptyImage Encrypted image where additional data will be embedded
         * @param t_Data Data to embed
         * @param t_DataEmbeddingKey key to use to embed data
         * @param t_MaxEmbeddingRate std::optional to use with benchmarks
         * @param t_MaxUserDataBits std::optional to use with benchmarks
         * @return Encrypted image with embedded data into it (Edited t_EncryptedImage).
        */

        static BmpImage& Embed(BmpImage& t_EncryptedImage, const std::vector<uint8_t>& t_Data, const std::vector<uint8_t>& t_DataEmbeddingKey, std::optional<std::reference_wrapper<double>> t_MaxEmbeddingRate, std::optional<std::reference_wrapper<uint32_t>> t_MaxUserDataBits);
    private:
        /**
         * @brief Fills matrix using data key, and PRNG.
         * @param t_Mat matrix to fill.
         * @param t_DataEmbeddingKey key to use, to setup PRNG.
        */
        static void PreparePseudoRandomMatrix(Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>& t_Mat, const std::vector<uint8_t>& t_DataEmbeddingKey);

        /**
         * @brief Generates hash for group G_i.
         * @param t_CurGroup group to calculate hash for.
         * @param t_DataEmbeddingKey data embedding key, which is used to generate PR matrix.
         * @param t_ReinitRandomMatrix Flag that signals, that we need to reinitialize random matrix. 
         * (added to make the benchmarks work correctly).
         * @return "BitString" that represents hash for the block.
        */
        static std::string HashLsbBlock(const Group& t_CurGroup, const std::vector<uint8_t>& t_DataEmbeddingKey, bool t_ReinitRandomMatrix);

        /**
         * @brief Compresses group t_LsbEncodedGroup using matrix multiplication.
         * @param t_Psi First matrix to multiply with.
         * @param t_LsbEncodedGroup actual vector, that we want to compress.
         * @param t_LsbEncodedBitStream where to append compressed result.
         * @param t_HashsesBitStream where to append group hash.
         * @param t_DataEmbeddingKey key to use for hash calculation.
         * @param t_ReinitRandomMatrix Flag that signals, that we need to reinitialize random matrix.
         * (added to make the benchmarks work correctly).
        */
        static void CompressCurrentGroup(const Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>& t_Psi, const Group& t_LsbEncodedGroup, std::string& t_LsbEncodedBitStream, std::string& t_HashsesBitStream, const std::vector<uint8_t>& t_DataEmbeddingKey, bool t_ReinitRandomMatrix);

        friend class Extractor;
    };
}
#include <bitset>
#include <cmath>
#include <random>

#include "embedder/embedder.h"
#include "embedder/huffman.h"
#include "embedder/consts.h"
#include "utils.h"

#include <boost/log/trivial.hpp>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <boost/compute/detail/sha1.hpp>

namespace rdh {
    BmpImage& Embedder::Embed(BmpImage& t_EncryptedImage, const std::vector<uint8_t>& t_Data, const std::vector<uint8_t>& t_DataEmbeddingKey)
    {
        /* Create Huffman-coder object, which will be used to encode RLC sequences */
        Huffman<std::pair<uint16_t, Color16s>, pair_hash> huffmanCoder(consts::c_DefaultNode);

        /* Set default frequencies (found using statistical approach) */
        huffmanCoder.SetFrequencies(consts::huffman::c_DefaultFrequencies);

        /* In the article it's referred as R. */
        uint32_t omegaOneBlocks{ 0 };

        /* In the article it's referred as L */
        uint32_t totalBlocks{ static_cast<std::size_t>(t_EncryptedImage.GetHeight()) * static_cast<std::size_t>(t_EncryptedImage.GetWidth()) / 4 };

        /* The article says that the length of each encoded block must be less than this number. */
        constexpr uint32_t rlcEncodedMaxSize = utils::math::CeilLog2(consts::c_Threshold);

        /**
         * For each block we should save its encoded content length. In the article it's referred as \Re.
         * Yep, that (and everything else) can be rewritten using custom bitstring 
         * class (or using boost::dynamic_bitset<>), consider creating a pull request.
         */
        std::string lengthsBitStream{ "" };
        lengthsBitStream.reserve(rlcEncodedMaxSize * static_cast<std::size_t>(totalBlocks * consts::c_RlcEncodedBlocksRatioAvg));

        /**
         * "Bitstring", that represents all concatenated and compressed with RLC-based algorithm blocks.
         * In the article it's referred as C. It's length referred as \eta_{1}.
         */
        std::string rlcEncodedBitStream{ "" };

        /* Bitstring with all lsbs of the top-left pixels. In the article it's referred as F. */
        std::string topLeftPixelsLsb{ "" };
        topLeftPixelsLsb.reserve(totalBlocks);

        /* Bitstream, that represents compressed blocks from \omega_2 */
        std::string lsbEncodedBitStream{ "" };
        lsbEncodedBitStream.reserve(
            static_cast<std::size_t>((consts::c_Lambda * (4 * consts::c_LsbLayers - 1) - consts::c_Alpha) * utils::math::Floor((float)totalBlocks * (float)consts::c_LsbEncodedBlocksRatioAvg / (float)consts::c_Lambda))
        );

        /* Number of rows in each group vector */
        constexpr uint32_t groupRowsCnt{ consts::c_Lambda * (4 * consts::c_LsbLayers - 1) };

        /* Vector of groups. In the article Group is referred as G_i. */
        using Group = Eigen::Matrix<uint8_t, consts::c_Lambda* (4 * consts::c_LsbLayers - 1), 1>;
        Group lsbEncodedGroup = Group::Zero(groupRowsCnt, 1);
        
        assert(lsbEncodedGroup.rows() == groupRowsCnt);
        assert(lsbEncodedGroup.cols() == 1);
        assert(lsbEncodedGroup.isZero(0));

        /* Used to keep track of current group size. */
        uint32_t currGroupSize{ 0 };

        /* In the article it's referred as P. Number of rows. */
        constexpr uint32_t matrixRows{ (uint32_t)consts::c_Lambda * ((uint32_t)consts::c_PixelsInOneBlock * (uint32_t)consts::c_LsbLayers - 1) - consts::c_Alpha };

        /* In the article it's referred as Q. Number of columns. */
        constexpr uint32_t matrixColumns{ (uint32_t)consts::c_Lambda * ((uint32_t)consts::c_PixelsInOneBlock * (uint32_t)consts::c_LsbLayers - 1) };

        /* In the article it's referred as \psi. */
        Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic> psi(matrixRows, matrixColumns);

        /* In the article it's referred as Z. Size of this matrix is P \times \alpha. */
        Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic> pseudoRandomMat(matrixRows, consts::c_Alpha);
        PreparePseudoRandomMatrix(pseudoRandomMat, t_DataEmbeddingKey);

        /* Create binary matrix as described in the article */
        psi << Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>::Identity(matrixRows, matrixRows), pseudoRandomMat;

        /* Iterate over 2x2 blocks to compress them. */
        for (uint32_t imgY = 0; imgY < t_EncryptedImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_EncryptedImage.GetWidth(); imgX += 2) {
                /**
                 * Get RLC-encoded representation of a block to determine if it can be 
                 * compressed using RLC-based algorithm, or we should use LSB-based one.
                 */
                std::string rlcEncoded = RlcEncoder::Encode(
                    t_EncryptedImage.GetPixel(imgY, imgX),
                    t_EncryptedImage.GetPixel(imgY, imgX + 1),
                    t_EncryptedImage.GetPixel(imgY + 1, imgX),
                    t_EncryptedImage.GetPixel(imgY + 1, imgX + 1), 
                    huffmanCoder
                );

                /* Save lsb of the top-left pixel in a block */
                topLeftPixelsLsb += (t_EncryptedImage.GetPixel(imgY, imgX) & 1) ? "1" : "0";

                /**
                 * Determine, if a block belongs to omega one or not.
                 * If so, append already computed representation of this block to the rlcEncodedBitStream.
                 * Also don't forget to append new value to the lengthsBitStream, and to update byte in the 
                 * locationMap.
                 */
                if (rlcEncoded.size() < consts::c_Threshold) {
                    /**
                     * Set lsb of top-left pixel. This bit is used to determine
                     * which approach (lsb/rlc) was used to encode the block.
                     */
                    t_EncryptedImage.SetPixel(imgY, imgX, t_EncryptedImage.GetPixel(imgY, imgX) | 1);

                    /* Append encoded block representation to the 'C' bitstream. */
                    rlcEncodedBitStream += rlcEncoded;

                    /* Append encoded block length to the '\Re' bitstream */
                    lengthsBitStream += std::bitset<rlcEncodedMaxSize>(rlcEncoded.size()).to_string();

                    omegaOneBlocks++;
                }
                else {
                    /* Clear lsb of top-left pixel. */
                    t_EncryptedImage.SetPixel(imgY, imgX, t_EncryptedImage.GetPixel(imgY, imgX) & ~1);

                    /**
                     * For each pixel extract required LSBs.
                     * Total iterations: 4 * c_LsbLayers - 1.
                     * In proposed schema, the last group may not be full! Is it okay?
                     */
                    for (uint32_t yAdd = 0; yAdd < 2; ++yAdd) {
                        for (uint32_t xAdd = 0; xAdd < 2; ++xAdd) {
                            uint8_t curPixel{ t_EncryptedImage.GetPixel(imgY + yAdd, imgX + xAdd) };
                            /* For the top-left pixel, we ignore it's first LSB */
                            for (uint32_t bitPos = (yAdd == 0 && xAdd == 0) ? 1 : 0; bitPos < consts::c_LsbLayers; bitPos++) {
                                lsbEncodedGroup(currGroupSize++, 0) = utils::math::GetNthBit(curPixel, bitPos);

                                /* If we've already exceed group size, so create new one. Also don't forget to reset currGroupSize. */
                                if (currGroupSize >= groupRowsCnt) {
                                    /**
                                     * Multiply matrices mod 2.
                                     * @TODO: Check if multiplication is done correctly!
                                     */
                                    Eigen::Matrix<uint8_t, matrixRows, 1> binaryColumnVec = (psi * lsbEncodedGroup).unaryExpr([](const uint8_t x) { return x % 2; });
                                    assert(binaryColumnVec.rows() == psi.rows());
                                    assert(binaryColumnVec.cols() == 1);

                                    for (uint32_t rowIdx = 0; rowIdx < binaryColumnVec.rows(); ++rowIdx) {
                                        lsbEncodedBitStream += (binaryColumnVec(rowIdx, 0) & 1) ? "1" : "0";
                                    }

                                    assert(currGroupSize == groupRowsCnt);
                                    lsbEncodedGroup.setZero();

                                    assert(lsbEncodedGroup.rows() == groupRowsCnt);
                                    assert(lsbEncodedGroup.cols() == 1);
                                    assert(lsbEncodedGroup.isZero(0));

                                    currGroupSize = 0;
                                }
                            }
                        }
                    }
                }
            }
        }

#ifndef NDEBUG
        uint32_t xi = utils::math::Floor((float)(totalBlocks - omegaOneBlocks) / (float)consts::c_Lambda);
        assert(lsbEncodedBitStream.size() == xi * (consts::c_Lambda * (4 * consts::c_LsbLayers - 1) - consts::c_Alpha));
#endif

        /* Last step. Pack all data into the image. */
        for (uint32_t imgY = 0; imgY < t_EncryptedImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_EncryptedImage.GetWidth(); imgX += 2) {
            }
        }

#if DEBUG_STATS == 1
        BOOST_LOG_TRIVIAL(info) << "Total blocks: " << totalBlocks;
        BOOST_LOG_TRIVIAL(info) << "Omega one blocks: " << omegaOneBlocks;
        BOOST_LOG_TRIVIAL(info) << "Ratio is (omegaOne/totalBlocks): " << (double)omegaOneBlocks / (double)totalBlocks;
#endif

        return t_EncryptedImage;
    }

    void Embedder::PreparePseudoRandomMatrix(Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>& t_Mat, const std::vector<uint8_t>& t_DataEmbeddingKey)
    {
        boost::uuids::detail::sha1 sha1;
        /* Calculate sha1 hash of t_DataEmbeddingKey, to use this hash as a seed for Mersenne twister PRNG. */
        sha1.process_bytes(t_DataEmbeddingKey.data(), t_DataEmbeddingKey.size());

        uint32_t hash[5] = { 0 };

        sha1.get_digest(hash);

        /* Build seed from hash. */
        std::seed_seq seq{ hash[0], hash[1], hash[2], hash[3], hash[4] };

        std::mt19937 generator(seq);
        std::uniform_int_distribution<short> dis(0, 1);

        /* Generate pseudo-random matrix of 1's and 0's */
        t_Mat = Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>::Zero(t_Mat.rows(), t_Mat.cols()).unaryExpr([&](uint8_t) { return dis(generator); });
    }
}

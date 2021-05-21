#include <bitset>
#include <cmath>
#include <random>

#include "embedder/embedder.h"
#include "embedder/compressor.h"
#include "embedder/huffman.h"
#include "embedder/consts.h"
#include "utils.h"

#include <boost/log/trivial.hpp>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>

namespace rdh {
    BmpImage& Embedder::Embed(BmpImage& t_EncryptedImage, const std::vector<uint8_t>& t_Data, const std::vector<uint8_t>& t_DataEmbeddingKey, std::optional<std::reference_wrapper<double>> t_MaxEmbeddingRate, std::optional<std::reference_wrapper<uint32_t>> t_MaxUserDataBits)
    {
        /* This is here only to allow matrix reinitialization while running benchmarks. */
        bool reinitRandomMatrixForHashCalculation{ true };

        Consts& constsRef = Consts::Instance();

        /* Create Huffman-coder object, which will be used to encode RLC sequences */
        Huffman<std::pair<uint16_t, Color16s>, pair_hash> huffmanCoder(consts::c_DefaultNode);

        /* Set default frequencies (found using statistical approach) */
        huffmanCoder.SetFrequencies(consts::huffman::c_DefaultFrequencies);

        /* In the article it's referred as R. */
        uint32_t omegaOneBlocks{ 0 };

        /* In the article it's referred as L */
        uint32_t totalBlocks{ 
            static_cast<std::size_t>(t_EncryptedImage.GetHeight()) * static_cast<std::size_t>(t_EncryptedImage.GetWidth()) / 4 
        };

        /**
         * For each block we should save its encoded content length. In the article it's referred as \Re.
         * Yep, that (and everything else) can be rewritten using custom bitstring 
         * class (or using boost::dynamic_bitset<>), consider creating a pull request.
         */
        std::string lengthsBitStream{ "" };
        lengthsBitStream.reserve(
            constsRef.GetRlcEncodedMaxSize() * static_cast<std::size_t>(totalBlocks * constsRef.GetRlcEncodedBlocksRatioAvg())
        );

        /**
         * "Bitstring", that represents all concatenated and compressed with RLC-based algorithm blocks.
         * In the article it's referred as C. It's length referred as \eta_{1}.
         */
        std::string rlcEncodedBitStream{ "" };
        rlcEncodedBitStream.reserve(
            static_cast<std::size_t>(totalBlocks * constsRef.GetRlcEncodedBlocksRatioAvg()) * constsRef.GetAvgRlcEncodedLength()
        );

        std::string hashsesBitStream{ "" };
        hashsesBitStream.reserve(
            utils::math::Floor((float)totalBlocks * constsRef.GetRlcEncodedBlocksRatioAvg() / (float)constsRef.GetLambda()) *
            constsRef.GetLsbHashSize()
        );

        /* Bitstring with all lsbs of the top-left pixels. In the article it's referred as F. */
        std::string topLeftPixelsLsbBitStream{ "" };
        topLeftPixelsLsbBitStream.reserve(totalBlocks);

        /* Bitstream, that represents compressed blocks from \omega_2 */
        std::string lsbEncodedBitStream{ "" };
        lsbEncodedBitStream.reserve(
            static_cast<std::size_t>((constsRef.GetGroupSizeBeforeCompression() - constsRef.GetAlpha()) * utils::math::Floor((float)totalBlocks * (float)constsRef.GetLsbEncodedBlocksRatioAvg() / (float)constsRef.GetLambda()))
        );

        /* Vector of groups. In the article Group is referred as G_i. */
        Group lsbCompressedGroup = Group::Zero(constsRef.GetGroupSizeBeforeCompression(), 1);
        
        assert(lsbCompressedGroup.rows() == constsRef.GetGroupSizeBeforeCompression());
        assert(lsbCompressedGroup.cols() == 1);
        assert(lsbCompressedGroup.isZero(0));

        /* Used to keep track of current group size. */
        uint32_t currGroupSize{ 0 };

        /* In the article it's referred as \psi. */
        Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic> psi(constsRef.GetGroupSizeAfterCompression(), constsRef.GetGroupSizeBeforeCompression());

        /* In the article it's referred as Z. Size of this matrix is P \times \alpha. */
        Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic> pseudoRandomMat(constsRef.GetGroupSizeAfterCompression(), constsRef.GetAlpha());
        PreparePseudoRandomMatrix(pseudoRandomMat, t_DataEmbeddingKey);

        /* Create binary matrix as described in the article */
        psi << Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>::Identity(constsRef.GetGroupSizeAfterCompression(), constsRef.GetGroupSizeAfterCompression()), pseudoRandomMat;

        /* Iterate over 2x2 blocks to compress them. */
        for (uint32_t imgY = 0; imgY < t_EncryptedImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_EncryptedImage.GetWidth(); imgX += 2) {
                /**
                 * Get RLC-encoded representation of a block to determine if it can be 
                 * compressed using RLC-based algorithm, or we should use LSB-based one.
                 */
                std::string rlcCompressed = RlcCompressor::Compress(
                    t_EncryptedImage.GetPixel(imgY, imgX),
                    t_EncryptedImage.GetPixel(imgY, imgX + 1),
                    t_EncryptedImage.GetPixel(imgY + 1, imgX),
                    t_EncryptedImage.GetPixel(imgY + 1, imgX + 1), 
                    huffmanCoder
                );

                /* Save lsb of the top-left pixel in a block */
                topLeftPixelsLsbBitStream += (t_EncryptedImage.GetPixel(imgY, imgX) & 1) ? "1" : "0";

                /**
                 * Determine, if a block belongs to omega one or not.
                 * If so, append already computed representation of this block to the rlcEncodedBitStream.
                 * Also don't forget to append new value to the lengthsBitStream, and to update byte in the 
                 * locationMap.
                 */
                if (rlcCompressed.size() < constsRef.GetThreshold()) {
                    /**
                     * Set lsb of top-left pixel. This bit is used to determine
                     * which approach (lsb/rlc) was used to encode the block.
                     */
                    t_EncryptedImage.SetPixel(imgY, imgX, t_EncryptedImage.GetPixel(imgY, imgX) | 1);

                    /* Append encoded block representation to the 'C' bitstream. */
                    rlcEncodedBitStream += rlcCompressed;

                    /* Append encoded block length to the '\Re' bitstream */
                    std::string encodedBlockStr;
                    boost::to_string(boost::dynamic_bitset<>(constsRef.GetRlcEncodedMaxSize(), rlcCompressed.size()), encodedBlockStr);
                    assert(encodedBlockStr.size() == constsRef.GetRlcEncodedMaxSize());
                    lengthsBitStream += encodedBlockStr;

                    /* Increase number of rlc-encoded blocks */
                    omegaOneBlocks++;
                }
                else {
                    /* Clear lsb of top-left pixel. */
                    t_EncryptedImage.SetPixel(imgY, imgX, t_EncryptedImage.GetPixel(imgY, imgX) & ~1);

                    /**
                     * For each pixel in a group of 4 pixels extract required LSBs.
                     * Total iterations: 4 * c_LsbLayers - 1.
                     */
                    for (uint32_t yAdd = 0; yAdd < 2; ++yAdd) {
                        for (uint32_t xAdd = 0; xAdd < 2; ++xAdd) {
                            uint8_t curPixel{ t_EncryptedImage.GetPixel(imgY + yAdd, imgX + xAdd) };
                            /* For the top-left pixel, we ignore it's first LSB */
                            uint32_t bitPos = ((yAdd == 0 && xAdd == 0) ? 1 : 0);
                            for (; bitPos < constsRef.GetLsbLayers(); bitPos++) {
                                lsbCompressedGroup(currGroupSize++, 0) = utils::math::GetNthBit(curPixel, bitPos);

                                /* If we've exceed group size - create a new one. Also don't forget to reset currGroupSize. */
                                if (currGroupSize >= constsRef.GetGroupSizeBeforeCompression()) {
                                    CompressCurrentGroup(psi, lsbCompressedGroup, lsbEncodedBitStream, hashsesBitStream, t_DataEmbeddingKey, reinitRandomMatrixForHashCalculation);
                                    reinitRandomMatrixForHashCalculation = false;

                                    assert(currGroupSize == constsRef.GetGroupSizeBeforeCompression());

                                    /* Reset group. */
                                    lsbCompressedGroup.setZero();

                                    /* Because Eigen is weird, lets just double check, if everything is done correctly. */
                                    assert(lsbCompressedGroup.rows() == constsRef.GetGroupSizeBeforeCompression());
                                    assert(lsbCompressedGroup.cols() == 1);
                                    assert(lsbCompressedGroup.isZero(0));

                                    /* Reset group size. */
                                    currGroupSize = 0;
                                }
                            }
                        }
                    }
                }
            }
        }

#if DEBUG_STATS == 1
        BOOST_LOG_TRIVIAL(info) << "Total blocks: " << totalBlocks;
        BOOST_LOG_TRIVIAL(info) << "Omega one blocks: " << omegaOneBlocks;
        BOOST_LOG_TRIVIAL(info) << "Ratio is (omegaOne/totalBlocks): " << (double)omegaOneBlocks / (double)totalBlocks;
        BOOST_LOG_TRIVIAL(info) << "Rlc-encoded bitstream length: " << rlcEncodedBitStream.size();
#endif

        double tMax = (
                24.0f * (double)omegaOneBlocks +
                std::floorf((totalBlocks - omegaOneBlocks) / (double)constsRef.GetLambda()) *
                ((double)constsRef.GetAlpha() - (double)constsRef.GetLsbHashSize()) -
                (double)omegaOneBlocks * std::ceilf(std::log2f(constsRef.GetThreshold())) -
                (double)rlcEncodedBitStream.size() -
                (double)totalBlocks
            ) / (
                (double)t_EncryptedImage.GetHeight() * (double)t_EncryptedImage.GetWidth()
            );

        BOOST_LOG_TRIVIAL(info) << "Maximum embedding rate: " << tMax;

        uint32_t xi = utils::math::Floor((float)(totalBlocks - omegaOneBlocks) / (float)constsRef.GetLambda());
        int32_t maxUserDataSize = 24 * omegaOneBlocks + xi * constsRef.GetLambda() * (4 * constsRef.GetLsbLayers() - 1)
            - (lengthsBitStream.size() + rlcEncodedBitStream.size() + lsbEncodedBitStream.size() + hashsesBitStream.size() + topLeftPixelsLsbBitStream.size());

        BOOST_LOG_TRIVIAL(info) << "Maximum bits of user-data to embed: " << maxUserDataSize;

        if (t_MaxEmbeddingRate != std::nullopt) {
            t_MaxEmbeddingRate->get() = tMax;
        }

        if (t_MaxUserDataBits != std::nullopt) {
            t_MaxUserDataBits->get() = maxUserDataSize;
        }

        if (tMax < 0.0f) {
            throw std::invalid_argument(
                "Incorrect parameters are set for data embedder! "
                "Consider using default parameters. Or find more appropriate values."
            );
        }

        if (t_Data.size() * 8 > maxUserDataSize) {
            throw std::invalid_argument("User data size exceeds the maximum possible size!");
        }

        /* Concatenate everything into a single BitStream */
        std::string userDataBitStream = utils::BytesToBinaryString(t_Data);

        assert(lengthsBitStream.size() == omegaOneBlocks * constsRef.GetRlcEncodedMaxSize());
        assert(lsbEncodedBitStream.size() == xi * (constsRef.GetLambda() * (4 * constsRef.GetLsbLayers() - 1) - constsRef.GetAlpha()));
        assert(hashsesBitStream.size() == constsRef.GetLsbHashSize() * xi);
        assert(topLeftPixelsLsbBitStream.size() == totalBlocks);

        assert(userDataBitStream.size() % 8 == 0);

        std::string assembledBitStream = 
            lengthsBitStream + rlcEncodedBitStream + lsbEncodedBitStream + 
            hashsesBitStream + topLeftPixelsLsbBitStream + userDataBitStream + std::string(maxUserDataSize - userDataBitStream.size(), '0');
        
        assert(assembledBitStream.size() == 24 * omegaOneBlocks + xi * constsRef.GetLambda() * (4 * constsRef.GetLsbLayers() - 1));
        
        std::array<uint32_t, 5> hash;

        /* Use sha1 of a data-hiding key as a seed for PRNG */
        utils::CalculateSHA1(t_DataEmbeddingKey, hash);
        std::seed_seq seq(hash.begin(), hash.end());
        
        /* Shuffle BitStream, before embedding */
        utils::ShuffleFisherYates(seq, assembledBitStream);

        auto sliceBegin = assembledBitStream.begin();
        auto sliceEnd = assembledBitStream.begin();

        /* Used to keep track of total written bits. For debug purposes. */
        uint32_t totalBitsWrittenRlc{ 0 };
        uint32_t totalBitsWrittenLsb{ 0 };

        /* Last step. Pack all data into the image. */
        for (uint32_t imgY = 0; imgY < t_EncryptedImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_EncryptedImage.GetWidth(); imgX += 2) {
                /* What type of block we are currently looking at? */
                if (t_EncryptedImage.GetPixel(imgY, imgX) & 1) {
                    assert(sliceBegin == sliceEnd);

                    /**
                     * The block is compressed using rlc-based algorithm, so we can fully use
                     * all of the pixels, excluding the top-left one.
                     */
                    t_EncryptedImage.SetPixel(imgY, imgX + 1, utils::BinaryStringToByte(
                        std::string(sliceBegin, utils::Advance(sliceEnd, assembledBitStream.end(), 8)))
                    );

                    t_EncryptedImage.SetPixel(imgY + 1, imgX, utils::BinaryStringToByte(
                        std::string(
                            utils::Advance(sliceBegin, assembledBitStream.end(), 8), 
                            utils::Advance(sliceEnd, assembledBitStream.end(), 8)
                        )
                    ));

                    t_EncryptedImage.SetPixel(imgY + 1, imgX + 1, utils::BinaryStringToByte(
                        std::string(
                            utils::Advance(sliceBegin, assembledBitStream.end(), 8),
                            utils::Advance(sliceEnd, assembledBitStream.end(), 8)
                        )
                    ));

                    totalBitsWrittenRlc += 24;

                    utils::Advance(sliceBegin, assembledBitStream.end(), 8);
                }
                else {
                    /* The block is encoded using lsb-based algorithm */
                    
                    /* Check, if we've filled all of the allowed LSB-encoded blocks with data */
                    if (totalBitsWrittenLsb >= xi * constsRef.GetLambda() * (4 * constsRef.GetLsbLayers() - 1)) {
                        continue;
                    }

                    assert(sliceBegin == sliceEnd);
                    
                    t_EncryptedImage.SetPixel(
                        imgY, 
                        imgX, 
                        utils::ClearLastNBits(t_EncryptedImage.GetPixel(imgY, imgX), constsRef.GetLsbLayers()) |
                        (utils::BinaryStringToByte(
                            std::string(
                                sliceBegin,
                                utils::Advance(sliceEnd, assembledBitStream.end(), constsRef.GetLsbLayers() - 1)
                            )
                        ) << 1)
                    );

                    t_EncryptedImage.SetPixel(
                        imgY,
                        imgX + 1,
                        utils::ClearLastNBits(t_EncryptedImage.GetPixel(imgY, imgX + 1), constsRef.GetLsbLayers()) |
                        utils::BinaryStringToByte(
                            std::string(
                                utils::Advance(sliceBegin, assembledBitStream.end(), constsRef.GetLsbLayers() - 1),
                                utils::Advance(sliceEnd, assembledBitStream.end(), constsRef.GetLsbLayers())
                            )
                        )
                    );

                    t_EncryptedImage.SetPixel(
                        imgY + 1,
                        imgX,
                        utils::ClearLastNBits(t_EncryptedImage.GetPixel(imgY + 1, imgX), constsRef.GetLsbLayers()) |
                        utils::BinaryStringToByte(
                            std::string(
                                utils::Advance(sliceBegin, assembledBitStream.end(), constsRef.GetLsbLayers()),
                                utils::Advance(sliceEnd, assembledBitStream.end(), constsRef.GetLsbLayers())
                            )
                        )
                    );

                    t_EncryptedImage.SetPixel(
                        imgY + 1,
                        imgX + 1,
                        utils::ClearLastNBits(t_EncryptedImage.GetPixel(imgY + 1, imgX + 1), constsRef.GetLsbLayers()) |
                        utils::BinaryStringToByte(
                            std::string(
                                utils::Advance(sliceBegin, assembledBitStream.end(), constsRef.GetLsbLayers()),
                                utils::Advance(sliceEnd, assembledBitStream.end(), constsRef.GetLsbLayers())
                            )
                        )
                    );

                    totalBitsWrittenLsb += (4 * constsRef.GetLsbLayers() - 1);

                    utils::Advance(sliceBegin, assembledBitStream.end(), constsRef.GetLsbLayers());
                }
            }
        }

        assert(sliceBegin == assembledBitStream.end());

        assert(totalBitsWrittenRlc == 24 * omegaOneBlocks);
        assert(totalBitsWrittenLsb == xi * constsRef.GetLambda() * (4 * constsRef.GetLsbLayers() - 1));
        assert(totalBitsWrittenRlc + totalBitsWrittenLsb == assembledBitStream.size());

        return t_EncryptedImage;
    }

    void Embedder::PreparePseudoRandomMatrix(Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>& t_Mat, const std::vector<uint8_t>& t_DataEmbeddingKey)
    {
        std::array<uint32_t, 5> hash;

        /* Calculate sha1 hash of t_DataEmbeddingKey, to use this hash as a seed for Mersenne twister PRNG. */
        utils::CalculateSHA1(t_DataEmbeddingKey, hash);

        /* Build seed from the hash. */
        std::seed_seq seq(hash.begin(), hash.end());

        std::mt19937 generator(seq);
        std::uniform_int_distribution<short> dis(0, 1);

        /* Generate pseudo-random matrix of 1's and 0's */
        t_Mat = Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>::Zero(t_Mat.rows(), t_Mat.cols()).unaryExpr([&](uint8_t) { return dis(generator); });
    }

    std::string Embedder::HashLsbBlock(const Group& t_CurGroup, const std::vector<uint8_t>& t_DataEmbeddingKey, bool t_ReinitRandomMatrix)
    {
        std::string hash{ "" };
        static Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic> randomMatrix(Consts::Instance().GetLsbHashSize(), Consts::Instance().GetGroupSizeBeforeCompression());

        if (t_ReinitRandomMatrix) {
            randomMatrix = Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>(Consts::Instance().GetLsbHashSize(), Consts::Instance().GetGroupSizeBeforeCompression());
            PreparePseudoRandomMatrix(randomMatrix, t_DataEmbeddingKey);
        }

        Eigen::Matrix<uint8_t, Eigen::Dynamic, 1> hashMatrix = (randomMatrix * t_CurGroup).unaryExpr([](const uint8_t x) { return x % 2; });

        assert(Consts::Instance().GetLsbHashSize() == hashMatrix.rows());

        for (uint32_t rowIdx = 0; rowIdx < hashMatrix.rows(); ++rowIdx) {
            hash += (hashMatrix(rowIdx, 0) & 1) ? "1" : "0";
        }

        return hash;
    }

    void Embedder::CompressCurrentGroup(const Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>& t_Psi, const Group& t_LsbEncodedGroup, std::string& t_LsbEncodedBitStream, std::string& t_HashsesBitStream, const std::vector<uint8_t>& t_DataEmbeddingKey, bool t_ReinitRandomMatrix)
    {
        /* Multiply matrices mod 2. */
        Eigen::Matrix<uint8_t, Eigen::Dynamic, 1> binaryColumnVec = (t_Psi * t_LsbEncodedGroup).unaryExpr([](const uint8_t x) { return x % 2; });
        assert(binaryColumnVec.rows() == t_Psi.rows());
        assert(binaryColumnVec.cols() == 1);

        /* Extract bits from binary column vector */
        for (uint32_t rowIdx = 0; rowIdx < binaryColumnVec.rows(); ++rowIdx) {
            t_LsbEncodedBitStream += (binaryColumnVec(rowIdx, 0) & 1) ? "1" : "0";
        }

        t_HashsesBitStream += HashLsbBlock(t_LsbEncodedGroup, t_DataEmbeddingKey, t_ReinitRandomMatrix);
    }
}

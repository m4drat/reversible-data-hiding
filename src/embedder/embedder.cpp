#include <bitset>
#include <cmath>

#include "embedder/embedder.h"
#include "embedder/huffman.h"
#include "embedder/consts.h"
#include "utils.h"

#include <boost/log/trivial.hpp>

namespace rdh {
    BmpImage Embedder::Embed(BmpImage& t_EncryptedImage, const std::vector<uint8_t>& t_Data, const std::vector<uint8_t>& t_DataEmbeddingKey)
    {
        BmpImage encryptedEmbedded(t_EncryptedImage.GetHeight(), t_EncryptedImage.GetWidth());

        /* In the article it's referred as R. */
        uint32_t omegaOneBlocks{ 0 };

        /* In the article it's referred as L */
        uint32_t totalBlocks{ static_cast<std::size_t>(t_EncryptedImage.GetHeight()) * static_cast<std::size_t>(t_EncryptedImage.GetWidth()) / 4 };

        /* The article says that the length of each encoded block must be less than this number. */
        constexpr uint32_t rlcEncodedMaxSize = utils::math::ceillog2(consts::c_Threshold);

        /**
         * For each block we should save its encoded content length. In the article it's referred as \Re.
         * Yep, that can be rewritten using custom bitstring class, consider creating a pull request.
         */
        std::string lengthsBitStream{ "" };
        lengthsBitStream.reserve(rlcEncodedMaxSize * static_cast<std::size_t>(totalBlocks * consts::c_RlcEncodedBlocksRatioAvg));

        /**
         * "Bitstring", that represents all concatenated and compressed with RLC-based algorithm blocks.
         * In the article it's referred as C. It's length referred as \eta_{1}.
         */
        std::string rlcEncodedBitStream{ "" };

        /* Bitstring with all lsbs of the top-left pixels. In the article it's referred as F. */
        std::string topLeftPixelsLsb(totalBlocks, '0');

        /* Create Huffman-coder object, which will be used to encode RLC sequences */
        Huffman<std::pair<uint16_t, Color16s>, pair_hash> huffmanCoder(consts::c_DefaultNode);
        /* Set default frequencies (found using statistical approach) */
        huffmanCoder.SetFrequencies(consts::huffman::c_DefaultFrequencies);

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

                /**
                 * Determine, if a block belongs to omega one or not.
                 * If so, append already computed representation of this block to the rlcEncodedBitStream.
                 * Also don't forget to append new value to the lengthsBitStream, and to update byte in the 
                 * locationMap.
                 */
                if (rlcEncoded.size() < consts::c_Threshold) {
                    /* Save lsb of the top-left pixel in a block */
                    topLeftPixelsLsb += (t_EncryptedImage.GetPixel(imgY, imgX) & 1) ? "1" : "0";

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
                    /**
                     * Block doesn't belong to omega_1 (isn't rlc-encoded).
                     * So encode it using LSB compression.
                     */
                    LsbEncoder::Encode(
                        t_EncryptedImage.GetPixel(imgY, imgX),
                        t_EncryptedImage.GetPixel(imgY, imgX + 1),
                        t_EncryptedImage.GetPixel(imgY + 1, imgX),
                        t_EncryptedImage.GetPixel(imgY + 1, imgX + 1)
                    );

                    /**
                     * Clear lsb of top-left pixel.
                     */
                    t_EncryptedImage.SetPixel(imgY, imgX, t_EncryptedImage.GetPixel(imgY, imgX) & ~1);
                }
            }
        }

        /**
         * Last step. Pack all data into the image.
         */
        for (uint32_t imgY = 0; imgY < t_EncryptedImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_EncryptedImage.GetWidth(); imgX += 2) {
            }
        }

#if DEBUG_STATS == 1
        BOOST_LOG_TRIVIAL(info) << "Total blocks: " << totalBlocks;
        BOOST_LOG_TRIVIAL(info) << "Omega one blocks: " << omegaOneBlocks;
        BOOST_LOG_TRIVIAL(info) << "Ratio is (omegaOne/totalBlocks): " << (double)omegaOneBlocks / (double)totalBlocks;
#endif

        return std::move(encryptedEmbedded);
    }
}

#include <bitset>
#include <cmath>

#include "embedder/embedder.h"
#include "embedder/encoded_block.h"
#include "embedder/huffman.h"
#include "embedder/consts.h"

#include <boost/log/trivial.hpp>

namespace rdh {
    BmpImage Embedder::Embed(const BmpImage& t_EncryptedImage, const std::vector<uint8_t>& t_Data, const std::vector<uint8_t>& t_DataEmbeddingKey)
    {
        BmpImage encryptedEmbedded(t_EncryptedImage.GetHeight(), t_EncryptedImage.GetWidth());

        // In the article it's referred as R.
        uint32_t omegaOneBlocks{ 0 };

        // In the article it's referred as L
        uint32_t totalBlocks{ static_cast<std::size_t>(t_EncryptedImage.GetHeight()) * static_cast<std::size_t>(t_EncryptedImage.GetWidth()) / 4 };

        // The article says that the length of each encoded block must be less than this number
        uint32_t rlcEncodedMaxSize = std::ceill(std::log2f(consts::c_Threshold));

        /**
         * For each block we should save its encoded content length. In the article it's referred as \Re.
         * Yep, that can be rewritten using custom bitstring class, consider creating a pull request.
         */
        std::string lengthsBitStream{ "" };

        /**
         * "Bitstring", that represents all encoded
         */
        std::string rlcEncodedBitStream{ "" };

        std::string locationMap(totalBlocks, '0');

        // A vector of EncodedBlocks. All of this blocks firstly will be encoded
        // using RLC-encoding + Huffman encoding.
        std::vector<EncodedBlockLsb> lsbEncodedBlocks;
        lsbEncodedBlocks.reserve(totalBlocks);

        // Create Huffman-coder object, which will be used to encode RLC sequences
        Huffman<std::pair<uint16_t, Color16s>, pair_hash> huffmanCoder(consts::c_DefaultNode);
        // Set default frequencies (found using statistical approach)
        huffmanCoder.SetFrequencies(consts::huffman::c_DefaultFrequencies);

        /**
         * Iterate over 2x2 blocks to compress them.
        */
        for (uint32_t imgY = 0; imgY < t_EncryptedImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_EncryptedImage.GetWidth(); imgX += 2) {
                std::string rlcEncoded = EncodedBlockRlc::EncodeRlc(
                    t_EncryptedImage.GetPixel(imgY, imgX),
                    t_EncryptedImage.GetPixel(imgY, imgX + 1),
                    t_EncryptedImage.GetPixel(imgY + 1, imgX),
                    t_EncryptedImage.GetPixel(imgY + 1, imgX + 1), 
                    huffmanCoder
                );

                /**
                 * Determine, if a block belongs to omega one or not.
                 * If so, use already computed representation of this block.
                 */
                if (rlcEncoded.size() < consts::c_Threshold) {
                    rlcEncodedBitStream += rlcEncoded;
                    //encodedBlocks.emplace_back(
                    //    EncodedBlockRlc(
                    //        t_EncryptedImage.GetPixel(imgY, imgX),
                    //        t_EncryptedImage.GetPixel(imgY, imgX + 1),
                    //        t_EncryptedImage.GetPixel(imgY + 1, imgX),
                    //        t_EncryptedImage.GetPixel(imgY + 1, imgX + 1),
                    //        huffmanCoder
                    //    )
                    //).SetEncoded(rlcEncoded);

                    ++omegaOneBlocks;
                }
                else {
                    /**
                     * Block doesn't belong to omega_1.
                     * So encode it using LSB compression.
                     */
                    encodedBlocks.emplace_back(
                        EncodedBlockLsb(
                            t_EncryptedImage.GetPixel(imgY, imgX),
                            t_EncryptedImage.GetPixel(imgY, imgX + 1),
                            t_EncryptedImage.GetPixel(imgY + 1, imgX),
                            t_EncryptedImage.GetPixel(imgY + 1, imgX + 1)
                        )
                    ).Encode();
                }
            }
        }

        /**
         * Last step. Pack all data into the image.
         */
        

#if DEBUG_STATS == 1
        BOOST_LOG_TRIVIAL(info) << "Total blocks: " << encodedBlocks.size();
        BOOST_LOG_TRIVIAL(info) << "Omega one blocks: " << omegaOneBlocks;
        BOOST_LOG_TRIVIAL(info) << "Ratio is (omegaOne/totalBlocks): " << (double)omegaOneBlocks / (double)encodedBlocks.size() << omegaOneBlocks;
#endif

        return std::move(encryptedEmbedded);
    }
}

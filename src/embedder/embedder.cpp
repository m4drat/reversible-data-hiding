#include "embedder/embedder.h"
#include "embedder/encoded_block.h"
#include "embedder/huffman.h"
#include "embedder/consts.h"

#include <boost/log/trivial.hpp>

namespace rdh {
    BmpImage Embedder::Embed(const BmpImage& t_EncryptedImage, const std::vector<uint8_t>& t_Data, const std::vector<uint8_t>& t_DataEmbeddingKey)
    {
        BmpImage encryptedEmbedded(t_EncryptedImage.GetHeight(), t_EncryptedImage.GetWidth());

        // A vector of EncodedBlocks. All of this blocks firstly will be encoded
        // using RLC-encoding + Huffman encoding.
        std::vector<EncodedBlock> encodedBlocks;
        encodedBlocks.reserve(static_cast<std::size_t>(t_EncryptedImage.GetHeight()) * static_cast<std::size_t>(t_EncryptedImage.GetWidth()) / 4);

        // Create Huffman-coder object, which will be used to encode RLC sequences
        Huffman<std::pair<uint16_t, Color16s>, pair_hash> huffmanCoder(std::pair<uint16_t, Color8u>(-1, 65535));
        // Set default frequencies (found using statistical approach)
        huffmanCoder.SetFrequencies(consts::huffman::c_DefaultFrequencies);

        /**
         * First step. Encode all 2x2 blocks using RLC encoding.
         */
        for (uint32_t imgY = 0; imgY < t_EncryptedImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_EncryptedImage.GetWidth(); imgX += 2) {
                encodedBlocks.emplace_back(
                    t_EncryptedImage.GetPixel(imgY, imgX), 
                    t_EncryptedImage.GetPixel(imgY, imgX + 1), 
                    t_EncryptedImage.GetPixel(imgY + 1, imgX),
                    t_EncryptedImage.GetPixel(imgY + 1, imgX + 1)
                );
            }
        }

#if DEBUG_STATS == 1
        uint32_t sigmaOneBlocks{ 0 };
#endif

        /**
         * Encoder Loop.
         */
        for (auto& encodedBlock : encodedBlocks) {
            /**
             * Second step. Encode each rlc-encoded block using Huffman coding.
             */
            if (encodedBlock.GetRlcEncoded().size() == 1 && encodedBlock.GetRlcEncoded().at(0) == std::pair<uint16_t, Color16s>(0, 0)) {
                /**
                 * Special case. All differences are equal to zero.
                 * In this scenario, huffman keyword length is 0.
                */
                encodedBlock.SetEncoded("");
            }
            else {
                // We know, that the last element will always be non-zero
                // @sa EncodedBlock::EncodedBlock
                encodedBlock.SetEncoded(huffmanCoder.Encode(encodedBlock.GetRlcEncoded()));
            }

            /**
             * Third step. Determine, if a block belongs to sigma one or not.
             */
            if (encodedBlock.GetEncoded().size() < consts::c_Threshold) {
                encodedBlock.SetIsSigmaOne(true);
#if DEBUG_STATS == 1
                ++sigmaOneBlocks;
#endif
            }
            else {
                /**
                 * Fourth step. Block doesn't belong to sigma_1.
                 * So encode it using LSB compression.
                 */
            }
        }

#if DEBUG_STATS == 1
        BOOST_LOG_TRIVIAL(info) << "Total blocks: " << encodedBlocks.size();
        BOOST_LOG_TRIVIAL(info) << "Sigma one blocks: " << sigmaOneBlocks;
        BOOST_LOG_TRIVIAL(info) << "Ratio is: " << (double)sigmaOneBlocks / (double)encodedBlocks.size() << sigmaOneBlocks;
#endif

        return std::move(encryptedEmbedded);
    }

    EncodedBlock& Embedder::EncodeBlockLsb(EncodedBlock& t_EncodedBlock)
    {
        
        return t_EncodedBlock;
    }
}

#include "embedder/embedder.h"
#include "embedder/encoded_block.h"
#include "embedder/huffman.h"
#include "embedder/consts.h"

#include <iostream>

namespace rdh {
    BmpImage Embedder::Embed(const BmpImage& t_EncryptedImage, const std::vector<uint8_t>& t_Data, const std::vector<uint8_t>& t_DataEmbeddingKey)
    {
        BmpImage encryptedEmbedded(t_EncryptedImage.GetHeight(), t_EncryptedImage.GetWidth());

        // A vector of EncodedBlock that represents rlc-encoded blocks.
        std::vector<EncodedBlock> encodedBlocks;
        encodedBlocks.reserve(static_cast<std::size_t>(t_EncryptedImage.GetHeight()) * static_cast<std::size_t>(t_EncryptedImage.GetWidth()) / 4);
        
        // Create Huffman-coder object, which will be used to encode RLC sequences
        Huffman<std::pair<uint16_t, Color16s>, pair_hash> huffmanCoder(std::pair<uint16_t, Color8>(-1, 65535));
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

        /**
         * Second step. Encode each rlc-encoded block using Huffman coding.
         */
        for (auto& encodedBlock : encodedBlocks) {
            /**
             * Third step (performed inside SetHuffmanEncoded). Determine, if block belongs to sigma one or not.
             */
            if (encodedBlock.GetRlcEncoded().size() == 1 && encodedBlock.GetRlcEncoded().at(0) == std::pair<uint16_t, Color16s>(0, 0)) {
                /**
                 * Special case. All differences are equal to zero.
                 * In this scenario, huffman keyword length is 0.
                */
                encodedBlock.SetHuffmanEncoded("");
            }
            else {
                // We know, that the last element will always be non-zero
                // @sa EncodedBlock::EncodedBlock
                encodedBlock.SetHuffmanEncoded(huffmanCoder.Encode(encodedBlock.GetRlcEncoded()));
            }
        }

        return std::move(encryptedEmbedded);
    }
}

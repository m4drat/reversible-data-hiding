#include "embedder/embedder.h"
#include "embedder/encoded_block.h"
#include "embedder/huffman.h"
#include "embedder/consts.h"

#include <iostream>

namespace rdh {
    BmpImage Embedder::Embed(const BmpImage& t_EncryptedImage, const std::vector<uint8_t>& t_Data, const std::vector<uint8_t>& t_DataEmbeddingKey)
    {
        // @TODO: Probably we can just edit passed image
        BmpImage encryptedEmbedded(t_EncryptedImage.GetHeight(), t_EncryptedImage.GetWidth());

        /**
        * A vector of vectors that represent rlc-encoded blocks.
        */
        std::vector<EncodedBlock> encodedBlocks;
        encodedBlocks.reserve(static_cast<std::size_t>(t_EncryptedImage.GetHeight()) * static_cast<std::size_t>(t_EncryptedImage.GetWidth()) / 4);
        
        /**
         * Create Huffman-coder object, which will be used to 
         * encode RLC sequences
        */
        Huffman<std::pair<uint16_t, Color16s>, pair_hash> huffmanCoder(std::pair<uint16_t, Color8>(-1, 65535));

        // Calculate RLC-related things
        for (uint32_t imgY = 0; imgY < t_EncryptedImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_EncryptedImage.GetWidth(); imgX += 2) {

                encodedBlocks.emplace_back(
                    t_EncryptedImage.GetPixel(imgY, imgX), 
                    t_EncryptedImage.GetPixel(imgY, imgX + 1), 
                    t_EncryptedImage.GetPixel(imgY + 1, imgX),
                    t_EncryptedImage.GetPixel(imgY + 1, imgX + 1)
                );

                /**
                 * Calculate number of occurrences of each symbol
                */
                for (const auto& pair_ : encodedBlocks.back().GetRlcEncoded()) {
                    huffmanCoder.UpdateFreqForSymbol(pair_);
                }
            }
        }

        for (auto& [pair_, code] : huffmanCoder.GetCodesTable()) {
            std::cout << "{ std::pair<uint16_t, Color8>(" << (uint32_t)pair_.first << ", " << (uint32_t)pair_.second << "), \"" << code << "\"}, " << std::endl;
        }

        // For each block find its representation using Huffman coding
        for (auto& encodedBlock : encodedBlocks) {
            encodedBlock.SetHuffmanEncoded(huffmanCoder.Encode(encodedBlock.GetRlcEncoded()));
        }

        return std::move(encryptedEmbedded);
    }
}

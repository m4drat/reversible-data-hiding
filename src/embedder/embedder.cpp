#include "embedder/embedder.h"
#include "embedder/encoded_block.h"
#include "embedder/huffman.h"

#include <iostream>

namespace rdh {
    BmpImage Embedder::Embed(const BmpImage& t_EncryptedImage, const std::vector<uint8_t>& t_Data, const std::vector<uint8_t>& t_DataEmbeddingKey)
    {
        // @TODO:
        // Probably we can just edit passed image
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
        Huffman<std::pair<uint16_t, Color8>, pair_hash> huffmanCoder(std::pair<uint16_t, Color8>(-1, 0));

        // Calculate RLC-related things
        for (uint32_t imgY = 0; imgY < t_EncryptedImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_EncryptedImage.GetWidth(); imgX += 2) {

                /**
                 * Even if difference is a negative number, we can represent it using unsigned value
                 */
                Color8 deltaM1 = t_EncryptedImage.GetPixel(imgY, imgX + 1) - t_EncryptedImage.GetPixel(imgY, imgX);
                Color8 deltaM2 = t_EncryptedImage.GetPixel(imgY + 1, imgX) - t_EncryptedImage.GetPixel(imgY, imgX);
                Color8 deltaM3 = t_EncryptedImage.GetPixel(imgY + 1, imgX + 1) - t_EncryptedImage.GetPixel(imgY, imgX);

                encodedBlocks.emplace_back(t_EncryptedImage.GetPixel(imgY, imgX), deltaM1, deltaM2, deltaM3);

                for (const auto& pair_ : encodedBlocks.back().GetRlcEncoded()) {
                    huffmanCoder.UpdateFreqForSymbol(pair_);
                }
            }
        }

        // For each block find its representation using Huffman-coding
        for (auto& EncodedBlock : encodedBlocks) {
            EncodedBlock.SetHuffmanEncoded(huffmanCoder.Encode(EncodedBlock.GetRlcEncoded()));
        }

        //for (auto& vec : rlcEncoded) {
        //    for (auto& pair_ : vec) {
        //        frequences[pair_]++;
        //    }
        //}

        //for (auto const& [key, val] : frequences) {
        //    std::cout << "(" << key.first << ", " << (int32_t)key.second << ") : " << val << std::endl;
        //}

        return std::move(encryptedEmbedded);
    }
}
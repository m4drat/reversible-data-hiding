#include "embedder/embedder.h"
#include "embedder/encoded_block.h"
#include "embedder/huffman.h"
#include "embedder/consts.h"

#include <iostream>

namespace rdh {
    BmpImage Embedder::Embed(const BmpImage& t_EncryptedImage, const std::vector<uint8_t>& t_Data, const std::vector<uint8_t>& t_DataEmbeddingKey)
    {
        BmpImage encryptedEmbedded(t_EncryptedImage.GetHeight(), t_EncryptedImage.GetWidth());

        /**
        * A vector of vectors that represent rlc-encoded blocks.
        */
        std::vector<EncodedBlock> encodedBlocks;
        encodedBlocks.reserve(static_cast<std::size_t>(t_EncryptedImage.GetHeight()) * static_cast<std::size_t>(t_EncryptedImage.GetWidth()) / 4);
        
        // Create Huffman-coder object, which will be used to encode RLC sequences
        Huffman<std::pair<uint16_t, Color16s>, pair_hash> huffmanCoder(std::pair<uint16_t, Color8>(-1, 65535));
        huffmanCoder.SetFrequencies(consts::huffman::c_DefaultFrequencies);

        // Calculate RLC-related things
        for (uint32_t imgY = 0; imgY < t_EncryptedImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_EncryptedImage.GetWidth(); imgX += 2) {
                encodedBlocks.emplace_back(
                    t_EncryptedImage.GetPixel(imgY, imgX), 
                    t_EncryptedImage.GetPixel(imgY, imgX + 1), 
                    t_EncryptedImage.GetPixel(imgY + 1, imgX),
                    t_EncryptedImage.GetPixel(imgY + 1, imgX + 1)
                );

                // Calculate number of occurrences of each symbol
                //for (const auto& pair_ : encodedBlocks.back().GetRlcEncoded()) {
                //    if (pair_ != std::pair<uint16_t, Color16s>(0, 0)) {
                //        huffmanCoder.UpdateFreqForSymbol(pair_);
                //    }
                //}
            }
        }

        //std::vector<std::pair<std::pair<uint16_t, Color16s>, uint32_t>> freqs(huffmanCoder.GetFrequencies().begin(), huffmanCoder.GetFrequencies().end());
        //std::sort(freqs.begin(), freqs.end(), [](
        //    const std::pair<std::pair<uint16_t, Color16s>, uint32_t>& a,
        //    const std::pair<std::pair<uint16_t, Color16s>, uint32_t>& b
        //    ) {
        //        return a.second > b.second;
        //    });
        //std::cout << "Encoded blocks size: " << encodedBlocks.size() << std::endl;
        //std::cout << "Size: " << freqs.size() << "/1533" << std::endl;
        //for (auto& pair_ : freqs) {
        //    // { std::pair<uint16_t, Color16s>
        //    std::cout << "(" << (uint32_t)pair_.first.first << ", " << (int32_t)pair_.first.second << ") : " << pair_.second << ", " << std::endl;
        //}

        // For each block find its representation using Huffman coding
        for (auto& encodedBlock : encodedBlocks) {
            if (encodedBlock.GetRlcEncoded().size() == 1 && encodedBlock.GetRlcEncoded().at(0) == std::pair<uint16_t, Color16s>(0, 0)) {
                /**
                 * Special case. All differences are equal to zero.
                 * In this scenario, huffman keyword length is 0.
                */
                encodedBlock.SetHuffmanEncoded("");
            }
            else if (encodedBlock.GetRlcEncoded().size() != 1 && encodedBlock.GetRlcEncoded().back() == std::pair<uint16_t, Color16s>(0, 0)) {
                /**
                 * Second special case. Last block is (0, 0). This case should be encoded
                 * using huffman encoding only for non "zero"
                */
            }
            else {
                encodedBlock.SetHuffmanEncoded(huffmanCoder.Encode(encodedBlock.GetRlcEncoded()));
            }
            
        }

        return std::move(encryptedEmbedded);
    }
}

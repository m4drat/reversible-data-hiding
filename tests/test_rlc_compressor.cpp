#include "gtest/gtest.h"

#include "types.h"
#include "embedder/compressor.h"
#include "embedder/huffman.h"
#include "embedder/consts.h"

using namespace rdh;

TEST(RlcCompressTest, AllDifferencesAreZeroes_test) {
    /* Create Huffman-coder object, which will be used to encode RLC sequences */
    Huffman<std::pair<uint16_t, Color16s>, pair_hash> huffmanCoder(consts::c_DefaultNode);

    /* Set default frequencies (found using statistical approach) */
    huffmanCoder.SetFrequencies(consts::huffman::c_DefaultFrequencies);
    
    std::vector<Color8u> origPixels{ 112, 112, 112, 112 };

    /* Compressor test */
    std::string compressed{ RlcCompressor::Compress(origPixels[0], origPixels[1], origPixels[2], origPixels[3], huffmanCoder) };

    /* Decompressor test */
    std::vector<Color8u> decompressedPixels{ RlcCompressor::Decompress(origPixels[0], compressed, huffmanCoder) };
    ASSERT_EQ(origPixels.size(), decompressedPixels.size());
    for (uint32_t i = 0; i < std::max(origPixels.size(), decompressedPixels.size()); ++i) {
        ASSERT_EQ(origPixels.at(i), decompressedPixels.at(i));
    }
}

TEST(RlcCompressTest, LastDifferenceIsZero_test) {
    /* Create Huffman-coder object, which will be used to encode RLC sequences */
    Huffman<std::pair<uint16_t, Color16s>, pair_hash> huffmanCoder(consts::c_DefaultNode);

    /* Set default frequencies (found using statistical approach) */
    huffmanCoder.SetFrequencies(consts::huffman::c_DefaultFrequencies);

    std::vector<Color8u> origPixels{ 115, 79, 180, 115 };

    /* Compressor test */
    std::string compressed{ RlcCompressor::Compress(origPixels[0], origPixels[1], origPixels[2], origPixels[3], huffmanCoder) };

    /* Decompressor test */
    std::vector<Color8u> decompressedPixels{ RlcCompressor::Decompress(origPixels[0], compressed, huffmanCoder) };
    ASSERT_EQ(origPixels.size(), decompressedPixels.size());
    for (uint32_t i = 0; i < std::max(origPixels.size(), decompressedPixels.size()); ++i) {
        ASSERT_EQ(origPixels.at(i), decompressedPixels.at(i));
    }
}

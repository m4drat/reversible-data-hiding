#include "gtest/gtest.h"

#include <unordered_map>

#include "embedder/huffman.h"
#include "types.h"

using namespace rdh;

TEST(HuffmanTest, EncodeString_test) {
    Huffman<char, std::hash<char>> huffmanCoder('\0');
    huffmanCoder.SetFrequencies(
        std::unordered_map<char, uint32_t, std::hash<char>>{
            std::make_pair('A', 50),
            std::make_pair('B', 30),
            std::make_pair('C', 15),
            std::make_pair('D', 5)
        }
    );

    ASSERT_EQ(huffmanCoder.Encode(std::vector<char>{'A', 'A', 'A', 'B', 'C', 'C', 'D'}), "00011101101100");
}

TEST(HuffmanTest, EncodePairs_test) {
    Huffman<std::pair<uint16_t, Color8>, pair_hash> huffmanCoder(std::pair<uint16_t, Color8>(-1, 0));
    huffmanCoder.SetFrequencies(
        std::unordered_map<std::pair<uint16_t, Color8>, uint32_t, pair_hash>{
            std::make_pair(std::make_pair(0, 0), 50),
            std::make_pair(std::make_pair(0, 1), 30),
            std::make_pair(std::make_pair(0, 2), 15),
            std::make_pair(std::make_pair(0, 3), 5)
        }
    );

    ASSERT_EQ(huffmanCoder.Encode(
        std::vector<std::pair<uint16_t, Color8>>{
            std::make_pair(0, 0), std::make_pair(0, 0), std::make_pair(0, 0), 
            std::make_pair(0, 1), std::make_pair(0, 2), std::make_pair(0, 2), 
            std::make_pair(0, 3)
        }), "00011101101100");
}

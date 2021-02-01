#include "gtest/gtest.h"

#include <unordered_map>

#include "embedder/huffman.h"

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

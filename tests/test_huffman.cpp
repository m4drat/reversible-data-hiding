#include "gtest/gtest.h"

#include <unordered_map>

#include "embedder/huffman.h"
#include "types.h"

using namespace rdh;

TEST(HuffmanTest, EncodeDecodeString_test) {
    Huffman<char, std::hash<char>> huffmanCoder('\0');
    huffmanCoder.SetFrequencies(
        std::unordered_map<char, uint32_t, std::hash<char>>{
            std::make_pair('A', 50),
            std::make_pair('B', 30),
            std::make_pair('C', 15),
            std::make_pair('D', 5)
        }
    );

    std::string encoded = "00011101101100";
    std::vector<char> original{'A', 'A', 'A', 'B', 'C', 'C', 'D'};

    ASSERT_EQ(huffmanCoder.Encode(original), encoded);

    std::vector<char> decoded = huffmanCoder.Decode(encoded);

    ASSERT_EQ(decoded.size(), original.size());

    for (uint32_t i = 0; i < decoded.size(); ++i) {
        ASSERT_EQ(decoded.at(i), original.at(i));
    }
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

    std::string encoded = "00011101101100";
    std::vector<std::pair<uint16_t, Color8>> original{
        std::make_pair(0, 0), std::make_pair(0, 0), std::make_pair(0, 0),
        std::make_pair(0, 1), std::make_pair(0, 2), std::make_pair(0, 2),
        std::make_pair(0, 3)
    };

    ASSERT_EQ(huffmanCoder.Encode(original), encoded);

    std::vector<std::pair<uint16_t, Color8>> decoded = huffmanCoder.Decode(encoded);

    ASSERT_EQ(decoded.size(), original.size());

    for (uint32_t i = 0; i < decoded.size(); ++i) {
        ASSERT_EQ(decoded.at(i), original.at(i));
    }
}

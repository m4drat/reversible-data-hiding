#include "gtest/gtest.h"

#include "types.h"
#include "embedder/rlc.h"

using namespace rdh;

TEST(RLCEncoderTest, TwoZeroElements_test) {
    std::vector <std::pair<uint16_t, Color8>> encoded{ RLC::RlcEncode<uint16_t, Color8>({ 0, (Color8)-1, 0 }) };
    std::vector <std::pair<uint16_t, Color8>> expected{ { 1, 0xff }, { 0, 0 } };

    ASSERT_EQ(encoded.size(), expected.size());

    for (uint32_t i = 0; i < std::max(encoded.size(), expected.size()); ++i) {
        ASSERT_EQ(encoded.at(i), expected.at(i));
    }
}

TEST(RLCEncoderTest, ThreeZeroElements_test) {
    std::vector <std::pair<uint16_t, Color8>> encoded{ RLC::RlcEncode<uint16_t, Color8>({ 0, 0, 0 }) };
    std::vector <std::pair<uint16_t, Color8>> expected{ { 2, 0 } };

    ASSERT_EQ(encoded.size(), expected.size());

    for (uint32_t i = 0; i < std::max(encoded.size(), expected.size()); ++i) {
        ASSERT_EQ(encoded.at(i), expected.at(i));
    }
}

TEST(RLCEncoderTest, FirstZeroElement_test) {
    std::vector <std::pair<uint16_t, Color8>> encoded{ RLC::RlcEncode<uint16_t, Color8>({ 0, 1, 2 }) };
    std::vector <std::pair<uint16_t, Color8>> expected{ { 1, 1 }, { 0, 2 } };

    ASSERT_EQ(encoded.size(), expected.size());

    for (uint32_t i = 0; i < std::max(encoded.size(), expected.size()); ++i) {
        ASSERT_EQ(encoded.at(i), expected.at(i));
    }
}

TEST(RLCEncoderTest, LastZeroElement_test) {
    std::vector <std::pair<uint16_t, Color8>> encoded{ RLC::RlcEncode<uint16_t, Color8>({ 1, 2, 0 }) };
    std::vector <std::pair<uint16_t, Color8>> expected{ { 0, 1 }, { 0, 2 }, { 0, 0 } };

    ASSERT_EQ(encoded.size(), expected.size());

    for (uint32_t i = 0; i < std::max(encoded.size(), expected.size()); ++i) {
        ASSERT_EQ(encoded.at(i), expected.at(i));
    }
}

TEST(RLCEncoderTest, AllNonZero_test) {
    std::vector <std::pair<uint16_t, Color8>> encoded{ RLC::RlcEncode<uint16_t, Color8>({ 1, 2, 3 }) };
    std::vector <std::pair<uint16_t, Color8>> expected{ { 0, 1 }, { 0, 2 }, { 0, 3 } };

    ASSERT_EQ(encoded.size(), expected.size());

    for (uint32_t i = 0; i < std::max(encoded.size(), expected.size()); ++i) {
        ASSERT_EQ(encoded.at(i), expected.at(i));
    }
}

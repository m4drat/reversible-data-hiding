#include "gtest/gtest.h"

#include "types.h"
#include "embedder/rlc.h"

using namespace rdh;

TEST(RLCEncoderTest, TwoZeroElements_test) {
    std::vector <std::pair<uint16_t, Color16s>> encoded{ RLC::RlcEncode<uint16_t, Color16s>({ 0, (Color16s)-1, 0 }) };
    std::vector <std::pair<uint16_t, Color16s>> expected{ { 1, -1 }, { 0, 0 } };

    ASSERT_EQ(encoded.size(), expected.size());

    for (uint32_t i = 0; i < std::max(encoded.size(), expected.size()); ++i) {
        ASSERT_EQ(encoded.at(i), expected.at(i));
    }
}

TEST(RLCEncoderTest, ThreeZeroElements_test) {
    std::vector <std::pair<uint16_t, Color16s>> encoded{ RLC::RlcEncode<uint16_t, Color16s>({ 0, 0, 0 }) };
    std::vector <std::pair<uint16_t, Color16s>> expected{ { 2, 0 } };

    ASSERT_EQ(encoded.size(), expected.size());

    for (uint32_t i = 0; i < std::max(encoded.size(), expected.size()); ++i) {
        ASSERT_EQ(encoded.at(i), expected.at(i));
    }
}

TEST(RLCEncoderTest, ZeroElementIsInTheMiddle_test) {
    std::vector <std::pair<uint16_t, Color16s>> encoded{ RLC::RlcEncode<uint16_t, Color16s>({ 1, 0, 1 }) };
    std::vector <std::pair<uint16_t, Color16s>> expected{ { 0, 1 }, {1, 1} };

    ASSERT_EQ(encoded.size(), expected.size());

    for (uint32_t i = 0; i < std::max(encoded.size(), expected.size()); ++i) {
        ASSERT_EQ(encoded.at(i), expected.at(i));
    }
}

TEST(RLCEncoderTest, FirstZeroElement_test) {
    std::vector <std::pair<uint16_t, Color16s>> encoded{ RLC::RlcEncode<uint16_t, Color16s>({ 0, 1, 2 }) };
    std::vector <std::pair<uint16_t, Color16s>> expected{ { 1, 1 }, { 0, 2 } };

    ASSERT_EQ(encoded.size(), expected.size());

    for (uint32_t i = 0; i < std::max(encoded.size(), expected.size()); ++i) {
        ASSERT_EQ(encoded.at(i), expected.at(i));
    }
}

TEST(RLCEncoderTest, LastZeroElement_test) {
    std::vector <std::pair<uint16_t, Color16s>> encoded{ RLC::RlcEncode<uint16_t, Color16s>({ 1, 2, 0 }) };
    std::vector <std::pair<uint16_t, Color16s>> expected{ { 0, 1 }, { 0, 2 }, { 0, 0 } };

    ASSERT_EQ(encoded.size(), expected.size());

    for (uint32_t i = 0; i < std::max(encoded.size(), expected.size()); ++i) {
        ASSERT_EQ(encoded.at(i), expected.at(i));
    }
}

TEST(RLCEncoderTest, AllNonZero_test) {
    std::vector <std::pair<uint16_t, Color8u>> encoded{ RLC::RlcEncode<uint16_t, Color8u>({ 1, 2, 3 }) };
    std::vector <std::pair<uint16_t, Color8u>> expected{ { 0, 1 }, { 0, 2 }, { 0, 3 } };

    ASSERT_EQ(encoded.size(), expected.size());

    for (uint32_t i = 0; i < std::max(encoded.size(), expected.size()); ++i) {
        ASSERT_EQ(encoded.at(i), expected.at(i));
    }
}

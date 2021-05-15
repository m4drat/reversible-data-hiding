#include "gtest/gtest.h"

#include <random>

#include "types.h"
#include "embedder/embedder.h"
#include "utils.h"

using namespace rdh;

TEST(UtilsTest, FisherYates_test) {
    std::string original = "ABCDEFGHIJKLMNOPQRST9165078234";
    std::string shuffled = original;
    std::seed_seq seq{ 1337, 1338, 1447, 1588, 1922 };

    utils::ShuffleFisherYates(seq, shuffled);
    ASSERT_NE(shuffled, original);
    std::string deshuffl = shuffled;

    utils::DeshuffleFisherYates(seq, deshuffl);
    ASSERT_EQ(deshuffl, original);
}

TEST(UtilsTest, ClearBits_test) {
    uint8_t num = 0b10110101;
    ASSERT_EQ(utils::ClearLastNBits(num, 0), 0b10110101);
    ASSERT_EQ(utils::ClearLastNBits(num, 1), 0b10110100);
    ASSERT_EQ(utils::ClearLastNBits(num, 2), 0b10110100);
    ASSERT_EQ(utils::ClearLastNBits(num, 3), 0b10110000);
    ASSERT_EQ(utils::ClearLastNBits(num, 4), 0b10110000);
    ASSERT_EQ(utils::ClearLastNBits(num, 5), 0b10100000);
    ASSERT_EQ(utils::ClearLastNBits(num, 6), 0b10000000);
    ASSERT_EQ(utils::ClearLastNBits(num, 7), 0b10000000);
    ASSERT_EQ(utils::ClearLastNBits(num, 8), 0b00000000);
}

TEST(UtilsTest, Floor_test) {
    ASSERT_EQ(utils::math::Floor(0.0), 0);
    ASSERT_EQ(utils::math::Floor(0.499999), 0);
    ASSERT_EQ(utils::math::Floor(0.5), 0);
    ASSERT_EQ(utils::math::Floor(0.999999), 0);
    ASSERT_EQ(utils::math::Floor(1.0), 1);
    ASSERT_EQ(utils::math::Floor(123.0), 123);
    ASSERT_EQ(utils::math::Floor(123.4), 123);
}

TEST(UtilsTest, Ceil_test) {
    ASSERT_EQ(utils::math::Ceil(0.0), 0);
    ASSERT_EQ(utils::math::Ceil(0.499999), 1);
    ASSERT_EQ(utils::math::Ceil(0.5), 1);
    ASSERT_EQ(utils::math::Ceil(0.999999), 1);
    ASSERT_EQ(utils::math::Ceil(1.0), 1);
    ASSERT_EQ(utils::math::Ceil(123.0), 123);
    ASSERT_EQ(utils::math::Ceil(123.4), 124);
}

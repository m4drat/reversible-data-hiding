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

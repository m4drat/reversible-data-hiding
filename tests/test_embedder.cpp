#include "gtest/gtest.h"

#include "types.h"
#include "embedder/embedder.h"

using namespace rdh;

TEST(EmbedderTest, Image_4x4px_test) {
    BmpImage image(ImageMatrix<Color8u>({
        {0x0, 0x10, 0x20, 0x30},
        {0x40, 0x50, 0x60, 0x70},
        {0x80, 0x90, 0xa0, 0xb0},
        {0xc0, 0xd0, 0xe0, 0xf0}
    }));
}

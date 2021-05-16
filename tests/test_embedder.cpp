#include "gtest/gtest.h"

#include "types.h"
#include "embedder/embedder.h"

using namespace rdh;

TEST(EmbedderTest, Image_4x4px_test) {
    /**
     * 1st block - LSB
     * 2nd block - RLC
     * 3rd block - RLC
     * 4th block - LSB
     */
    BmpImage image(ImageMatrix<Color8u>({
        {0b00000001, 0b11111001, 0b00100000, 0b00100001},
        {0b01001001, 0b11110000, 0b00100011, 0b00100100},
        {0b10100000, 0b10100011, 0b00000000, 0b10110001},
        {0b10100001, 0b10100100, 0b00010101, 0b00111111}
    }));

    /**
     * 1st block RLC-encoded: 
     * @param  
     * @param  
    */
    BmpImage imageReference(ImageMatrix<Color8u>({
        {0b00000000, 0b11111001, 0b00100000, 0b00100001},
        {0b01001001, 0b11110000, 0b00100011, 0b00100100},
        {0b10100000, 0b10100011, 0b00000000, 0b10110001},
        {0b10100001, 0b10100100, 0b00010101, 0b00111111}
    }));

    std::vector<uint8_t> data{ 0b11010010 };
    std::vector<uint8_t> dataEmbedkey{ 0x11, 0x12, 0x13, 0x14 };

    Consts::Instance().UpdateLambda(25);

    Embedder::Embed(image, data, dataEmbedkey);

    for (uint32_t imgY = 0; imgY < image.GetHeight(); imgY += 2) {
        for (uint32_t imgX = 0; imgX < image.GetWidth(); imgX += 2) {
            ASSERT_EQ(image.GetPixel(imgY, imgX), imageReference.GetPixel(imgY, imgX));
        }
    }
}

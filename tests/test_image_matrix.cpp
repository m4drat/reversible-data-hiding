#include "gtest/gtest.h"

#include "image_matrix.h"

using namespace rdh;

TEST(ImageMatrixTest, DefaultConstructor_test) {
    ImageMatrix<Color8> imMat;
    ASSERT_EQ(0, imMat.GetHeight());
    ASSERT_EQ(0, imMat.GetWidth());
}
TEST(ImageMatrixTest, InitializerListInitialization_test) {
    ImageMatrix<Color8> imMat({
        {0x0, 0x10, 0x20, 0x30},
        {0x40, 0x50, 0x60, 0x70},
        {0x80, 0x90, 0xa0, 0xb0},
        {0xc0, 0xd0, 0xe0, 0xf0}
    });

    // Check first row
    ASSERT_EQ(0x0, imMat.GetPixel(0, 0));
    ASSERT_EQ(0x10, imMat.GetPixel(0, 1));
    ASSERT_EQ(0x20, imMat.GetPixel(0, 2));
    ASSERT_EQ(0x30, imMat.GetPixel(0, 3));

    // Check second row
    ASSERT_EQ(0x40, imMat.GetPixel(1, 0));
    ASSERT_EQ(0x50, imMat.GetPixel(1, 1));
    ASSERT_EQ(0x60, imMat.GetPixel(1, 2));
    ASSERT_EQ(0x70, imMat.GetPixel(1, 3));

    // Check third row
    ASSERT_EQ(0x80, imMat.GetPixel(2, 0));
    ASSERT_EQ(0x90, imMat.GetPixel(2, 1));
    ASSERT_EQ(0xa0, imMat.GetPixel(2, 2));
    ASSERT_EQ(0xb0, imMat.GetPixel(2, 3));

    // Check fourth row
    ASSERT_EQ(0xc0, imMat.GetPixel(3, 0));
    ASSERT_EQ(0xd0, imMat.GetPixel(3, 1));
    ASSERT_EQ(0xe0, imMat.GetPixel(3, 2));
    ASSERT_EQ(0xf0, imMat.GetPixel(3, 3));
}

TEST(ImageMatrixTest, WidthHeightColorConstructor_test) {
    ImageMatrix<Color8> imMat(1, 2, 0xff);
    ASSERT_EQ(1, imMat.GetHeight());
    ASSERT_EQ(2, imMat.GetWidth());
    ASSERT_EQ(0xff, imMat.GetPixel(0, 0));
    ASSERT_EQ(0xff, imMat.GetPixel(0, 1));
}

TEST(ImageMatrixTest, SetPixel_test) {
    ImageMatrix<Color8> imMat(4, 4, 0x0);
    ASSERT_EQ(0, imMat.GetPixel(0, 0));
    imMat.SetPixel(0, 0, 0xff);
    ASSERT_EQ(0xff, imMat.GetPixel(0, 0));
}

TEST(ImageMatrixTest, GetPixel_test) {
    ImageMatrix<Color8> imMat(4, 4, 0xff);
    ASSERT_EQ(0xff, imMat.GetPixel(0, 0));
}

TEST(ImageMatrixTest, Slice_test) {
    ImageMatrix<Color8> imMat({
        {0x0, 0x10, 0x20, 0x30},
        {0x40, 0x50, 0x60, 0x70},
        {0x80, 0x90, 0xa0, 0xb0},
        {0xc0, 0xd0, 0xe0, 0xf0}
    });

    ImageMatrix<Color8> sliced = imMat.Slice(0, 1, 0, 1);

    ASSERT_EQ(0x0, imMat.GetPixel(0, 0));
    ASSERT_EQ(0x10, imMat.GetPixel(0, 1));
    ASSERT_EQ(0x40, imMat.GetPixel(1, 0));
    ASSERT_EQ(0x50, imMat.GetPixel(1, 1));
}

#include "gtest/gtest.h"

#include "encryptor/encryptor.h"
#include "image/bmp_image.h"
#include "image/image_matrix.h"

using namespace rdh;

//TEST(EncryptorTest, CalculateKeyIndex_test) {
//    ASSERT_EQ(0, Encryptor::CalculateKeyIndex(4, 0, 0));
//    ASSERT_EQ(1, Encryptor::CalculateKeyIndex(4, 0, 1));
//    ASSERT_EQ(2, Encryptor::CalculateKeyIndex(4, 1, 0));
//    ASSERT_EQ(3, Encryptor::CalculateKeyIndex(4, 1, 1));
//
//    ASSERT_EQ(16, Encryptor::CalculateKeyIndex(14, 0, 8));
//    ASSERT_EQ(17, Encryptor::CalculateKeyIndex(14, 0, 9));
//    ASSERT_EQ(20, Encryptor::CalculateKeyIndex(14, 0, 10));
//    ASSERT_EQ(21, Encryptor::CalculateKeyIndex(14, 0, 11));
//
//    ASSERT_EQ(34, Encryptor::CalculateKeyIndex(14, 3, 2));
//    ASSERT_EQ(35, Encryptor::CalculateKeyIndex(14, 3, 3));
//    ASSERT_EQ(38, Encryptor::CalculateKeyIndex(14, 3, 4));
//    ASSERT_EQ(39, Encryptor::CalculateKeyIndex(14, 3, 5));
//}

TEST(EncryptorTest, EncryptImage_test) {
    BmpImage image(ImageMatrix<Color8u>({
        {0x0, 0x10, 0x20, 0x30},
        {0x40, 0x50, 0x60, 0x70},
        {0x80, 0x90, 0xa0, 0xb0},
        {0xc0, 0xd0, 0xe0, 0xf0}
    }));

    std::vector<uint8_t> encryptionKey{ 
        0x10, 0x34, 0x11
    };

    BmpImage encryptedImage = Encryptor::Encrypt(image, encryptionKey);

    ASSERT_EQ(encryptionKey[0] ^ image.GetPixel(0, 0), encryptedImage.GetPixel(0, 0));
    ASSERT_EQ(encryptionKey[0] ^ image.GetPixel(0, 1), encryptedImage.GetPixel(0, 1));
    ASSERT_EQ(encryptionKey[0] ^ image.GetPixel(1, 0), encryptedImage.GetPixel(1, 0));
    ASSERT_EQ(encryptionKey[0] ^ image.GetPixel(1, 1), encryptedImage.GetPixel(1, 1));

    ASSERT_EQ(encryptionKey[1] ^ image.GetPixel(0, 2), encryptedImage.GetPixel(0, 2));
    ASSERT_EQ(encryptionKey[1] ^ image.GetPixel(0, 3), encryptedImage.GetPixel(0, 3));
    ASSERT_EQ(encryptionKey[1] ^ image.GetPixel(1, 2), encryptedImage.GetPixel(1, 2));
    ASSERT_EQ(encryptionKey[1] ^ image.GetPixel(1, 3), encryptedImage.GetPixel(1, 3));

    ASSERT_EQ(encryptionKey[2] ^ image.GetPixel(2, 0), encryptedImage.GetPixel(2, 0));
    ASSERT_EQ(encryptionKey[2] ^ image.GetPixel(2, 1), encryptedImage.GetPixel(2, 1));
    ASSERT_EQ(encryptionKey[2] ^ image.GetPixel(3, 0), encryptedImage.GetPixel(3, 0));
    ASSERT_EQ(encryptionKey[2] ^ image.GetPixel(3, 1), encryptedImage.GetPixel(3, 1));

    ASSERT_EQ(encryptionKey[0] ^ image.GetPixel(2, 2), encryptedImage.GetPixel(2, 2));
    ASSERT_EQ(encryptionKey[0] ^ image.GetPixel(2, 3), encryptedImage.GetPixel(2, 3));
    ASSERT_EQ(encryptionKey[0] ^ image.GetPixel(3, 2), encryptedImage.GetPixel(3, 2));
    ASSERT_EQ(encryptionKey[0] ^ image.GetPixel(3, 3), encryptedImage.GetPixel(3, 3));
}

TEST(EncryptorTest, DecryptImage_test) {
    BmpImage originalImage(ImageMatrix<Color8u>({
        {0x0, 0x10, 0x20, 0x30},
        {0x40, 0x50, 0x60, 0x70},
        {0x80, 0x90, 0xa0, 0xb0},
        {0xc0, 0xd0, 0xe0, 0xf0}
    }));

    BmpImage encryptedImage(ImageMatrix<Color8u>({
        { 0x10, 0x0,  0x14, 0x4 },
        { 0x50, 0x40, 0x54, 0x44 },
        { 0x91, 0x81, 0xb0, 0xa0 },
        { 0xd1, 0xc1, 0xf0, 0xe0 }
    }));

    std::vector<uint8_t> encryptionKey{
        0x10, 0x34, 0x11
    };

    BmpImage decryptedImage = Encryptor::Decrypt(encryptedImage, encryptionKey);

    ASSERT_EQ(originalImage.GetPixel(0, 0), decryptedImage.GetPixel(0, 0));
    ASSERT_EQ(originalImage.GetPixel(0, 1), decryptedImage.GetPixel(0, 1));
    ASSERT_EQ(originalImage.GetPixel(1, 0), decryptedImage.GetPixel(1, 0));
    ASSERT_EQ(originalImage.GetPixel(1, 1), decryptedImage.GetPixel(1, 1));

    ASSERT_EQ(originalImage.GetPixel(0, 2), decryptedImage.GetPixel(0, 2));
    ASSERT_EQ(originalImage.GetPixel(0, 3), decryptedImage.GetPixel(0, 3));
    ASSERT_EQ(originalImage.GetPixel(1, 2), decryptedImage.GetPixel(1, 2));
    ASSERT_EQ(originalImage.GetPixel(1, 3), decryptedImage.GetPixel(1, 3));

    ASSERT_EQ(originalImage.GetPixel(2, 0), decryptedImage.GetPixel(2, 0));
    ASSERT_EQ(originalImage.GetPixel(2, 1), decryptedImage.GetPixel(2, 1));
    ASSERT_EQ(originalImage.GetPixel(3, 0), decryptedImage.GetPixel(3, 0));
    ASSERT_EQ(originalImage.GetPixel(3, 1), decryptedImage.GetPixel(3, 1));

    ASSERT_EQ(originalImage.GetPixel(2, 2), decryptedImage.GetPixel(2, 2));
    ASSERT_EQ(originalImage.GetPixel(2, 3), decryptedImage.GetPixel(2, 3));
    ASSERT_EQ(originalImage.GetPixel(3, 2), decryptedImage.GetPixel(3, 2));
    ASSERT_EQ(originalImage.GetPixel(3, 3), decryptedImage.GetPixel(3, 3));
}

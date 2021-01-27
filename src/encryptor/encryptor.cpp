#include "encryptor/encryptor.h"

#include <thread>
#include <immintrin.h>

namespace rdh {

    BmpImage Encryptor::Encrypt(const BmpImage& t_PlainImage, std::vector<uint8_t>& t_EncryptionKey)
    {
        assert(t_PlainImage.GetHeight() % 2 == 0);
        assert(t_PlainImage.GetWidth() % 2 == 0);

        BmpImage encryptedImage(t_PlainImage.GetHeight(), t_PlainImage.GetWidth());

        // If image width and height are larger than 1024x1024, use threaded version of encryptor.
        //if (t_PlainImage.GetHeight() > 1024 && t_PlainImage.GetWidth() > 1024) {
        //    auto [regionWidth, regionHeight, maxThreads] = t_PlainImage.OptimalSubdivision(std::thread::hardware_concurrency());

        //    std::vector<std::thread> threadpool;

        //    for (uint32_t currentRegionYStart = 0; currentRegionYStart < t_PlainImage.GetHeight(); currentRegionYStart += regionHeight) {
        //        for (uint32_t currentRegionXStart = 0; currentRegionXStart < t_PlainImage.GetWidth(); currentRegionXStart += regionWidth) {
        //            threadpool.emplace_back(EncryptorWorker, std::ref(t_PlainImage), std::ref(encryptedImage), t_EncryptionKey, currentRegionYStart, currentRegionYStart + regionHeight, currentRegionXStart, currentRegionXStart + regionWidth);
        //        }
        //    }

        //    for (auto& th : threadpool) {
        //        th.join();
        //    }
        //}
        //else {
        uint32_t keyCursor = 0;
        for (uint32_t imgY = 0; imgY < t_PlainImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_PlainImage.GetWidth(); imgX += 2) {
                /**
                    * Pixels layout in 2x2 block
                    *     0   1
                    * 0 | A | B |
                    * 1 | C | D |
                    */
                Color8 encPixelA = t_PlainImage.GetPixel(imgY, imgX) ^ t_EncryptionKey[keyCursor % t_EncryptionKey.size()];
                Color8 encPixelB = t_PlainImage.GetPixel(imgY, imgX + 1) ^ t_EncryptionKey[keyCursor % t_EncryptionKey.size()];
                Color8 encPixelC = t_PlainImage.GetPixel(imgY + 1, imgX) ^ t_EncryptionKey[keyCursor % t_EncryptionKey.size()];
                Color8 encPixelD = t_PlainImage.GetPixel(imgY + 1, imgX + 1) ^ t_EncryptionKey[keyCursor % t_EncryptionKey.size()];

                encryptedImage.SetPixel(imgY, imgX, encPixelA);
                encryptedImage.SetPixel(imgY, imgX + 1, encPixelB);
                encryptedImage.SetPixel(imgY + 1, imgX, encPixelC);
                encryptedImage.SetPixel(imgY + 1, imgX + 1, encPixelD);

                ++keyCursor;
            }
        }
        //}

        return std::move(encryptedImage);
    }
    BmpImage Encryptor::Decrypt(const BmpImage& t_EncryptedImage, std::vector<uint8_t>& t_DecryptionKey)
    {
        // Because XOR encryption is a symmetric crypto algorithm, Encryption and Decryption
        // are the same operations
        return std::move(Encryptor::Encrypt(t_EncryptedImage, t_DecryptionKey));
    }

    uint32_t Encryptor::CalculateKeyIndex(uint32_t t_ImageWidth, uint32_t t_Y, uint32_t t_X)
    {
        assert(false && "incorrect implementation");
        return (t_Y % 2) ? ((t_Y - 1) * t_ImageWidth + 2 * (1 + t_X) - t_X % 2) : (t_Y * t_ImageWidth + 2 * t_X - t_X % 2);
    }

    void Encryptor::EncryptorWorker(const BmpImage& t_PlainImage, BmpImage& t_EncryptedImage, const std::vector<uint8_t>& t_EncryptionKey, uint32_t t_YStart, uint32_t t_YEnd, uint32_t t_XStart, uint32_t t_XEnd)
    {
        assert(false && "incorrect implementation");
        // If image width can be divided by 16, use SIMD version of algorithm
        for (uint32_t imgY = t_YStart; imgY < t_YEnd; ++imgY) {
            for (uint32_t imgX = t_XStart; imgX < t_XEnd; ++imgX) {
                // Even if we are not blocking this with mutexes, it should be fine, because vector is already resized
                t_EncryptedImage.SetPixel(imgY, imgX, t_PlainImage.GetPixel(imgY, imgX) ^ t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX) % t_EncryptionKey.size()]);
            }
        }
    }
}

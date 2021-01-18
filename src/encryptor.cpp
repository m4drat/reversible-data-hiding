#include "encryptor.h"

#include <thread>
#include <immintrin.h>

namespace rdh {

    BmpImage Encryptor::Encrypt(const BmpImage& t_PlainImage, std::vector<uint8_t>& t_EncryptionKey)
    {
        assert(t_PlainImage.GetHeight() % 2 == 0);
        assert(t_PlainImage.GetWidth() % 2 == 0);

        BmpImage encryptedImage(t_PlainImage.GetHeight(), t_PlainImage.GetWidth());

        // If image width and height are larger than 1024x1024, use threaded version of encryptor.
        if (t_PlainImage.GetHeight() > 1024 && t_PlainImage.GetWidth() > 1024) {
            auto [regionWidth, regionHeight, maxThreads] = t_PlainImage.OptimalSubdivision(std::thread::hardware_concurrency());

            std::vector<std::thread> threadpool;

            for (uint32_t currentRegionYStart = 0; currentRegionYStart < t_PlainImage.GetHeight(); currentRegionYStart += regionHeight) {
                for (uint32_t currentRegionXStart = 0; currentRegionXStart < t_PlainImage.GetWidth(); currentRegionXStart += regionWidth) {
                    threadpool.emplace_back(EncryptorWorker, std::ref(t_PlainImage), std::ref(encryptedImage), t_EncryptionKey, currentRegionYStart, currentRegionYStart + regionHeight, currentRegionXStart, currentRegionXStart + regionWidth);
                }
            }

            for (auto& th : threadpool) {
                th.join();
            }
        }
        else {
            if (t_PlainImage.GetWidth() >= 32) { // Use SIMD
                for (uint32_t imgY = 0; imgY < t_PlainImage.GetHeight(); ++imgY) { // For each row, split it into blocks of 16 pixels, and process them
                    for (uint32_t imgX = 0; imgX < t_PlainImage.GetWidth(); imgX += 32) {
                        __m256i keyRow = _mm256_set_epi8(
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 1) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 2) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 3) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 4) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 5) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 6) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 7) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 8) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 9) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 10) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 11) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 12) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 13) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 14) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 15) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 16) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 17) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 18) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 19) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 20) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 21) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 22) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 23) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 24) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 25) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 26) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 27) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 28) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 29) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 30) % t_EncryptionKey.size()],
                            t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX + 31) % t_EncryptionKey.size()]
                        );
                        __m256i imageRow = _mm256_set_epi8(
                            t_PlainImage.GetPixel(imgY, imgX),      t_PlainImage.GetPixel(imgY, imgX + 1),  t_PlainImage.GetPixel(imgY, imgX + 2),  t_PlainImage.GetPixel(imgY, imgX + 3),
                            t_PlainImage.GetPixel(imgY, imgX + 4),  t_PlainImage.GetPixel(imgY, imgX + 5),  t_PlainImage.GetPixel(imgY, imgX + 6),  t_PlainImage.GetPixel(imgY, imgX + 7),
                            t_PlainImage.GetPixel(imgY, imgX + 8),  t_PlainImage.GetPixel(imgY, imgX + 9),  t_PlainImage.GetPixel(imgY, imgX + 10), t_PlainImage.GetPixel(imgY, imgX + 11),
                            t_PlainImage.GetPixel(imgY, imgX + 12), t_PlainImage.GetPixel(imgY, imgX + 13), t_PlainImage.GetPixel(imgY, imgX + 14), t_PlainImage.GetPixel(imgY, imgX + 15),
                            t_PlainImage.GetPixel(imgY, imgX + 16), t_PlainImage.GetPixel(imgY, imgX + 17), t_PlainImage.GetPixel(imgY, imgX + 18), t_PlainImage.GetPixel(imgY, imgX + 19),
                            t_PlainImage.GetPixel(imgY, imgX + 20), t_PlainImage.GetPixel(imgY, imgX + 21), t_PlainImage.GetPixel(imgY, imgX + 22), t_PlainImage.GetPixel(imgY, imgX + 23),
                            t_PlainImage.GetPixel(imgY, imgX + 24), t_PlainImage.GetPixel(imgY, imgX + 25), t_PlainImage.GetPixel(imgY, imgX + 26), t_PlainImage.GetPixel(imgY, imgX + 27),
                            t_PlainImage.GetPixel(imgY, imgX + 28), t_PlainImage.GetPixel(imgY, imgX + 29), t_PlainImage.GetPixel(imgY, imgX + 30), t_PlainImage.GetPixel(imgY, imgX + 31)
                        );

                        imageRow = _mm256_xor_si256(imageRow, keyRow);

                        encryptedImage.SetPixel(imgY, imgX + 31, imageRow.m256i_u8[0]);
                        encryptedImage.SetPixel(imgY, imgX + 30, imageRow.m256i_u8[1]);
                        encryptedImage.SetPixel(imgY, imgX + 29, imageRow.m256i_u8[2]);
                        encryptedImage.SetPixel(imgY, imgX + 28, imageRow.m256i_u8[3]);
                        encryptedImage.SetPixel(imgY, imgX + 27, imageRow.m256i_u8[4]);
                        encryptedImage.SetPixel(imgY, imgX + 26, imageRow.m256i_u8[5]);
                        encryptedImage.SetPixel(imgY, imgX + 25, imageRow.m256i_u8[6]);
                        encryptedImage.SetPixel(imgY, imgX + 24, imageRow.m256i_u8[7]);
                        encryptedImage.SetPixel(imgY, imgX + 23, imageRow.m256i_u8[8]);
                        encryptedImage.SetPixel(imgY, imgX + 22, imageRow.m256i_u8[9]);
                        encryptedImage.SetPixel(imgY, imgX + 21, imageRow.m256i_u8[10]);
                        encryptedImage.SetPixel(imgY, imgX + 20, imageRow.m256i_u8[11]);
                        encryptedImage.SetPixel(imgY, imgX + 19, imageRow.m256i_u8[12]);
                        encryptedImage.SetPixel(imgY, imgX + 18, imageRow.m256i_u8[13]);
                        encryptedImage.SetPixel(imgY, imgX + 17, imageRow.m256i_u8[14]);
                        encryptedImage.SetPixel(imgY, imgX + 16, imageRow.m256i_u8[15]);
                        encryptedImage.SetPixel(imgY, imgX + 15, imageRow.m256i_u8[16]);
                        encryptedImage.SetPixel(imgY, imgX + 14, imageRow.m256i_u8[17]);
                        encryptedImage.SetPixel(imgY, imgX + 13, imageRow.m256i_u8[18]);
                        encryptedImage.SetPixel(imgY, imgX + 12, imageRow.m256i_u8[19]);
                        encryptedImage.SetPixel(imgY, imgX + 11, imageRow.m256i_u8[20]);
                        encryptedImage.SetPixel(imgY, imgX + 10, imageRow.m256i_u8[21]);
                        encryptedImage.SetPixel(imgY, imgX + 9,  imageRow.m256i_u8[22]);
                        encryptedImage.SetPixel(imgY, imgX + 8,  imageRow.m256i_u8[23]);
                        encryptedImage.SetPixel(imgY, imgX + 7,  imageRow.m256i_u8[24]);
                        encryptedImage.SetPixel(imgY, imgX + 6,  imageRow.m256i_u8[25]);
                        encryptedImage.SetPixel(imgY, imgX + 5,  imageRow.m256i_u8[26]);
                        encryptedImage.SetPixel(imgY, imgX + 4,  imageRow.m256i_u8[27]);
                        encryptedImage.SetPixel(imgY, imgX + 3,  imageRow.m256i_u8[28]);
                        encryptedImage.SetPixel(imgY, imgX + 2,  imageRow.m256i_u8[29]);
                        encryptedImage.SetPixel(imgY, imgX + 1,  imageRow.m256i_u8[30]);
                        encryptedImage.SetPixel(imgY, imgX,      imageRow.m256i_u8[31]);
                    }
                }
            } else { // Set pixel-by-pixel
                for (uint32_t imgY = 0; imgY < t_PlainImage.GetHeight(); ++imgY) {
                    for (uint32_t imgX = 0; imgX < t_PlainImage.GetWidth(); ++imgX) {
                        Color8 encPixel = t_PlainImage.GetPixel(imgY, imgX) ^ t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX) % t_EncryptionKey.size()];
                        encryptedImage.SetPixel(imgY, imgX, encPixel);
                    }
                }
            }
        }

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
        return (t_Y % 2) ? ((t_Y - 1) * t_ImageWidth + 2 * (1 + t_X) - t_X % 2) : (t_Y * t_ImageWidth + 2 * t_X - t_X % 2);
    }

    void Encryptor::EncryptorWorker(const BmpImage& t_PlainImage, BmpImage& t_EncryptedImage, const std::vector<uint8_t>& t_EncryptionKey, uint32_t t_YStart, uint32_t t_YEnd, uint32_t t_XStart, uint32_t t_XEnd)
    {
        // If image width can be divided by 16, use SIMD version of algorithm
        for (uint32_t imgY = t_YStart; imgY < t_YEnd; ++imgY) {
            for (uint32_t imgX = t_XStart; imgX < t_XEnd; ++imgX) {
                // Even if we are not blocking this with mutexes, it should be fine, because vector is already resized
                t_EncryptedImage.SetPixel(imgY, imgX, t_PlainImage.GetPixel(imgY, imgX) ^ t_EncryptionKey[CalculateKeyIndex(t_PlainImage.GetWidth(), imgY, imgX) % t_EncryptionKey.size()]);
            }
        }
    }
}

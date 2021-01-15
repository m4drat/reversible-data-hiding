#include "encryptor.h"

namespace rdh {

    BmpImage Encryptor::Encrypt(const BmpImage& t_PlainImage, std::vector<uint8_t>& t_EncryptionKey)
    {
        assert(t_PlainImage.GetHeight() % 2 == 0);
        assert(t_PlainImage.GetWidth() % 2 == 0);

        BmpImage encryptedImage(t_PlainImage.GetHeight(), t_PlainImage.GetWidth());

        uint32_t keyCursor = 0;

        for (uint32_t imgY = 0; imgY < t_PlainImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_PlainImage.GetWidth(); imgX += 2) {
                /**
                 * Pixels layout in 2x2 block
                 *     0   1
                 * 0 | A | B |
                 * 1 | C | D |
                 */
                Color8 encPixelA = t_PlainImage.GetPixel(imgY, imgX)         ^ t_EncryptionKey[keyCursor++ % t_EncryptionKey.size()];
                Color8 encPixelB = t_PlainImage.GetPixel(imgY, imgX + 1)     ^ t_EncryptionKey[keyCursor++ % t_EncryptionKey.size()];
                Color8 encPixelC = t_PlainImage.GetPixel(imgY + 1, imgX)     ^ t_EncryptionKey[keyCursor++ % t_EncryptionKey.size()];
                Color8 encPixelD = t_PlainImage.GetPixel(imgY + 1, imgX + 1) ^ t_EncryptionKey[keyCursor++ % t_EncryptionKey.size()];

                encryptedImage.SetPixel(imgY, imgX, encPixelA);
                encryptedImage.SetPixel(imgY, imgX + 1, encPixelB);
                encryptedImage.SetPixel(imgY + 1, imgX, encPixelC);
                encryptedImage.SetPixel(imgY + 1, imgX + 1, encPixelD);
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
}

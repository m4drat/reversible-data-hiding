#include "embedder/embedder.h"

namespace rdh {
    BmpImage Embedder::Embed(const BmpImage& t_EncryptedEmptyImage, std::vector<uint8_t>& t_Data, std::vector<uint8_t>& t_DataEmbeddingKey)
    {
        // @TODO:
        // Probably we can just edit passed image
        BmpImage encryptedEmbedded(t_EncryptedEmptyImage.GetHeight(), t_EncryptedEmptyImage.GetWidth());

        for (uint32_t imgY = 0; imgY < t_EncryptedEmptyImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_EncryptedEmptyImage.GetWidth(); imgX += 2) {
                /**
                 * Pixels layout in 2x2 block
                 *     0   1
                 * 0 | A | B |
                 * 1 | C | D |
                 */
                Color8 DeltaM1 = t_EncryptedEmptyImage.GetPixel(0, 1) - t_EncryptedEmptyImage.GetPixel(0, 0);
                Color8 DeltaM2 = t_EncryptedEmptyImage.GetPixel(1, 1) - t_EncryptedEmptyImage.GetPixel(0, 0);
                Color8 DeltaM3 = t_EncryptedEmptyImage.GetPixel(1, 1) - t_EncryptedEmptyImage.GetPixel(0, 0);
            }
        }

        return std::move(encryptedEmbedded);
    }
}
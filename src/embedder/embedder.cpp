#include "embedder/embedder.h"

#include "embedder/rlc.h"

#include <iostream>

namespace rdh {
    BmpImage Embedder::Embed(const BmpImage& t_EncryptedImage, const std::vector<uint8_t>& t_Data, const std::vector<uint8_t>& t_DataEmbeddingKey)
    {
        // @TODO:
        // Probably we can just edit passed image
        BmpImage encryptedEmbedded(t_EncryptedImage.GetHeight(), t_EncryptedImage.GetWidth());

        /**
        * A vector of vectors that represent rlc-encoded blocks
        */
        std::vector<std::vector<std::pair<uint32_t, Color8s>>> rlcEncoded;
        rlcEncoded.reserve(static_cast<std::size_t>(t_EncryptedImage.GetHeight()) * static_cast<std::size_t>(t_EncryptedImage.GetWidth()) / 4);

        for (uint32_t imgY = 0; imgY < t_EncryptedImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_EncryptedImage.GetWidth(); imgX += 2) {
                /**
                 * Pixels layout in 2x2 block
                 *     0   1
                 * 0 | A | B |
                 * 1 | C | D |
                 */
                Color8s deltaM1 = t_EncryptedImage.GetPixel(imgY, imgX + 1) - t_EncryptedImage.GetPixel(imgY, imgX);
                Color8s deltaM2 = t_EncryptedImage.GetPixel(imgY + 1, imgX) - t_EncryptedImage.GetPixel(imgY, imgX);
                Color8s deltaM3 = t_EncryptedImage.GetPixel(imgY + 1, imgX + 1) - t_EncryptedImage.GetPixel(imgY, imgX);

                if (deltaM1 == 0 && deltaM2 == 0 && deltaM3 == 0) {
                    rlcEncoded.push_back({ std::make_pair(0, 0) });
                }
                else {
                    rlcEncoded.push_back(RLC::RlcEncode<Color8s>({ deltaM1 , deltaM2, deltaM3}));
                }
            }
        }

        //for (auto& vec : rlcEncoded) {
        //    for (auto& pair_ : vec) {
        //        frequences[pair_]++;
        //    }
        //}

        //for (auto const& [key, val] : frequences) {
        //    std::cout << "(" << key.first << ", " << (int32_t)key.second << ") : " << val << std::endl;
        //}

        return std::move(encryptedEmbedded);
    }
}
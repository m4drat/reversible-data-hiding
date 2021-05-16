#include "extractor/extractor.h"

namespace rdh {
    void Extractor::RecoverImage(
        BmpImage& t_MarkedEncryptedImage,
        const std::string t_RecoveredImagePath,
        std::vector<uint8_t>& t_EncryptionKey
    ) 
    {

    }

    void Extractor::ExtractData(
        BmpImage& t_MarkedEncryptedImage,
        const std::string t_ExtractedDataPath,
        std::vector<uint8_t>& t_DataEmbeddingKey
    )
    {

    }

    void Extractor::RecoverImageAndExract(
        BmpImage& t_MarkedEncryptedImage,
        const std::string t_RecoveredImagePath,
        const std::string t_ExtractedDataPath,
        std::vector<uint8_t>& t_DataEmbeddingKey,
        std::vector<uint8_t>& t_EncryptionKey
    ) 
    {

    }
}
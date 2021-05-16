#include "extractor/extractor.h"

#include <boost/dynamic_bitset/dynamic_bitset.hpp>

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
        /* Get reference to a consts object. */
        Consts& constsRef = Consts::Instance();

        /**
         * Total number of 2x2 pixels blocks. 
         * In the article it's referred as L.
         */
        uint32_t totalBlocks{
            static_cast<std::size_t>(t_MarkedEncryptedImage.GetHeight()) * static_cast<std::size_t>(t_MarkedEncryptedImage.GetWidth()) / 4
        };

        /** 
         * Number of blocks encoded using RLC-based algorithm.
         * In the article it's referred as R.
         */
        uint32_t omegaOneBlocks{ 0 };

        /* Extracted from image bitstream */
        std::string extractedBitStream{ "" };
        extractedBitStream.reserve(
            static_cast<std::size_t>(24 * totalBlocks * constsRef.GetRlcEncodedBlocksRatioAvg())
        );

        /* Used to keep track of current lsb encoded group size (in bits) */
        uint32_t currGroupSize{ 0 };

        std::string currentGroup(constsRef.GetGroupRowsCount(), '0');

        /* Divide image into blocks, and calculate number of omega one blocks */
        for (uint32_t imgY = 0; imgY < t_MarkedEncryptedImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_MarkedEncryptedImage.GetWidth(); imgX += 2) {
                /* What type of block we are currently looking at? */
                if (t_MarkedEncryptedImage.GetPixel(imgY, imgX) & 1) {
                    /* RLC-encoded block */
                    omegaOneBlocks++;

                    std::string decodedBlockStr;
                    boost::to_string(boost::dynamic_bitset<>(8, t_MarkedEncryptedImage.GetPixel(imgY, imgX + 1)), decodedBlockStr);
                    extractedBitStream += decodedBlockStr;
                    
                    boost::to_string(boost::dynamic_bitset<>(8, t_MarkedEncryptedImage.GetPixel(imgY + 1, imgX)), decodedBlockStr);
                    extractedBitStream += decodedBlockStr;
                    
                    boost::to_string(boost::dynamic_bitset<>(8, t_MarkedEncryptedImage.GetPixel(imgY + 1, imgX + 1)), decodedBlockStr);
                    extractedBitStream += decodedBlockStr;
                }
                else {
                    /**
                     * For each pixel in a group of 4 pixels extract required LSBs.
                     * Total iterations: 4 * c_LsbLayers - 1.
                     */
                    for (uint32_t yAdd = 0; yAdd < 2; ++yAdd) {
                        for (uint32_t xAdd = 0; xAdd < 2; ++xAdd) {
                            uint8_t curPixel{ t_MarkedEncryptedImage.GetPixel(imgY + yAdd, imgX + xAdd) };
                            /* For the top-left pixel, we ignore it's first LSB */
                            for (uint32_t bitPos = (yAdd == 0 && xAdd == 0) ? 1 : 0; bitPos < constsRef.GetLsbLayers(); bitPos++) {
                                currentGroup.at(currGroupSize++) = utils::math::GetNthBit(curPixel, bitPos) ? '1' : '0';

                                /* If we've already exceed group size, so create new one. Also don't forget to reset currGroupSize. */
                                if (currGroupSize >= constsRef.GetGroupRowsCount()) {
                                    extractedBitStream += currentGroup;

                                    currentGroup = std::move(std::string(constsRef.GetGroupRowsCount(), '0'));

                                    /* Reset group size. */
                                    currGroupSize = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
    
        std::array<uint32_t, 5> hash;

        /* Use sha1 of a data-hiding key as a seed for PRNG */
        utils::CalculateSHA1(t_DataEmbeddingKey, hash);
        std::seed_seq seq(hash.begin(), hash.end());

        /* Shuffle BitStream, before embedding */
        utils::DeshuffleFisherYates(seq, extractedBitStream);

        /* Find value of xi, to calculate total number of bits used for data embedding. */
        //uint32_t xi = utils::math::Floor((float)(totalBlocks - omegaOneBlocks) / (float)constsRef.GetLambda());
        //uint32_t totalNumberOfBitsUsedForDataEmbedding = 24 * omegaOneBlocks + (xi * constsRef.GetLambda() * (4 * constsRef.GetLsbLayers() - 1));
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
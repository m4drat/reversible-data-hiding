#include "extractor/extractor.h"

#include "embedder/huffman.h"

#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <boost/log/trivial.hpp>

namespace rdh {
    void Extractor::RecoverImage(
        BmpImage& t_MarkedEncryptedImage,
        const std::string t_RecoveredImagePath,
        std::vector<uint8_t>& t_EncryptionKey
    ) 
    {
        /* Get reference to a consts object. */
        Consts& constsRef = Consts::Instance();

        uint32_t keyCursor = 0;
        /** 
         * First direct decryption pass. 
         * Decrypts top-left pixel in RLC-compressed blocks.
         * Decrypts all pixels in LSB-compressed blocks.
         */
        for (uint32_t imgY = 0; imgY < t_MarkedEncryptedImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_MarkedEncryptedImage.GetWidth(); imgX += 2) {
                /* What type of block we are currently looking at? */
                if (t_MarkedEncryptedImage.GetPixel(imgY, imgX) & 1) {
                    /* RLC-compressed block, so decrypt only the first pixel */
                    Color8u decPixelA = t_MarkedEncryptedImage.GetPixel(imgY, imgX) ^ t_EncryptionKey[keyCursor % t_EncryptionKey.size()];

                    /* Update pixel value, and set location map bit */
                    t_MarkedEncryptedImage.SetPixel(imgY, imgX, decPixelA | 1);
                }
                else {
                    /* LSB-compressed block, so decrypt all pixels in current block */
                    Color8u decPixelA = t_MarkedEncryptedImage.GetPixel(imgY, imgX) ^ t_EncryptionKey[keyCursor % t_EncryptionKey.size()];
                    Color8u decPixelB = t_MarkedEncryptedImage.GetPixel(imgY, imgX + 1) ^ t_EncryptionKey[keyCursor % t_EncryptionKey.size()];
                    Color8u decPixelC = t_MarkedEncryptedImage.GetPixel(imgY + 1, imgX) ^ t_EncryptionKey[keyCursor % t_EncryptionKey.size()];
                    Color8u decPixelD = t_MarkedEncryptedImage.GetPixel(imgY + 1, imgX + 1) ^ t_EncryptionKey[keyCursor % t_EncryptionKey.size()];

                    t_MarkedEncryptedImage.SetPixel(imgY, imgX, decPixelA);
                    t_MarkedEncryptedImage.SetPixel(imgY, imgX + 1, decPixelB);
                    t_MarkedEncryptedImage.SetPixel(imgY + 1, imgX, decPixelC);
                    t_MarkedEncryptedImage.SetPixel(imgY + 1, imgX + 1, decPixelD);
                
                    /* reset location map pixel */
                    t_MarkedEncryptedImage.SetPixel(imgY, imgX, t_MarkedEncryptedImage.GetPixel(imgY, imgX) & ~1);
                }

                ++keyCursor;
            }
        }

        /**
         * Second direct decryption pass.
         * Decrypts down-right pixel in RLC-compressed blocks by averaging it's neighbors.
         */
        for (uint32_t imgY = 0; imgY < t_MarkedEncryptedImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_MarkedEncryptedImage.GetWidth(); imgX += 2) {
                /* What type of block we are currently looking at? */
                if (t_MarkedEncryptedImage.GetPixel(imgY, imgX) & 1) {
                    /* RLC-compressed block, so decrypt down-right pixel */
                    uint16_t avgPixelValue{ 0 };
                    uint8_t avgOfNPixels{ 1 };

                    /* This pixel exists always */
                    avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY, imgX);

                    if (imgX + 2 < t_MarkedEncryptedImage.GetWidth()) {
                        avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY, imgX + 2);
                        avgOfNPixels++;
                    }

                    if (imgY + 2 < t_MarkedEncryptedImage.GetHeight()) {
                        avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY + 2, imgX);
                        avgOfNPixels++;
                    }

                    if (imgY + 2 < t_MarkedEncryptedImage.GetHeight() && imgX + 2 < t_MarkedEncryptedImage.GetWidth()) {
                        avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY + 2, imgX + 2);
                        avgOfNPixels++;
                    }

                    /* Set new average pixel value */
                    t_MarkedEncryptedImage.SetPixel(imgY + 1, imgX + 1, avgPixelValue / avgOfNPixels);
                }
            }
        }

        /**
         * Third direct decryption pass.
         * Decrypts pixels from RLC-compressed blocks, that are located borders of the image.
         */
        for (uint32_t imgY = 0; imgY < t_MarkedEncryptedImage.GetHeight(); imgY += 2) {
            /* First or the last row */
            if (imgY == 0 || imgY == t_MarkedEncryptedImage.GetHeight() - 2) {
                for (uint32_t imgX = 0; imgX < t_MarkedEncryptedImage.GetWidth(); imgX += 2) {
                    /* What type of block we are currently looking at? */
                    if (t_MarkedEncryptedImage.GetPixel(imgY, imgX) & 1) {
                        uint16_t avgPixelValue = 0;
                        uint8_t avgOfNPixels{ 2 };

                        avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY, imgX);
                        avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY + 1, imgX + 1);

                        /* First row */
                        if (imgY == 0) {
                            if (imgX + 2 < t_MarkedEncryptedImage.GetWidth()) {
                                avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY, imgX + 2);
                                avgOfNPixels++;
                            }

                            /* Set new average pixel value */
                            t_MarkedEncryptedImage.SetPixel(imgY, imgX + 1, avgPixelValue / avgOfNPixels);

                            /**
                             * If we are not in the top-left block, calculate average pixel value 
                             * for the down-left pixel.
                             */
                            if (imgX != 0) {
                                avgPixelValue = t_MarkedEncryptedImage.GetPixel(imgY, imgX);
                                avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY + 1, imgX + 1);
                                avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY + 2, imgX);
                                avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY + 1, imgX - 1);
                                t_MarkedEncryptedImage.SetPixel(imgY + 1, imgX, avgPixelValue / 4);
                            }
                        }
                        else {
                            /* Last row */
                            if (imgX >= 2) {
                                avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY + 1, imgX - 1);
                                avgOfNPixels++;
                            }

                            /* Set new average pixel value */
                            t_MarkedEncryptedImage.SetPixel(imgY + 1, imgX, avgPixelValue / avgOfNPixels);

                            /**
                             * If we are not in the down-right block, calculate average pixel value
                             * for the down-right pixel.
                             */
                            if (imgX != t_MarkedEncryptedImage.GetWidth() - 2) {
                                avgPixelValue = t_MarkedEncryptedImage.GetPixel(imgY, imgX);
                                avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY + 1, imgX + 1);
                                avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY, imgX + 2);
                                avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY - 1, imgX + 1);
                                t_MarkedEncryptedImage.SetPixel(imgY, imgX + 1, avgPixelValue / 4);
                            }
                        }
                    }
                }
            }
            else {
                /* First or the last column */
                for (uint32_t imgX : std::array<uint32_t, 2>{ 0, t_MarkedEncryptedImage.GetWidth() - 2 }) {
                    /* What type of block we are currently looking at? */
                    if (t_MarkedEncryptedImage.GetPixel(imgY, imgX) & 1) {
                        uint16_t avgPixelValue = 0;
                        uint8_t avgOfNPixels{ 0 };

                        /* First column */
                        if (imgX == 0) {
                            avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY, imgX);
                            avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY + 1, imgX + 1);
                            avgOfNPixels += 2;

                            if (imgY + 2 < t_MarkedEncryptedImage.GetHeight()) {
                                avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY + 2, imgX);
                                avgOfNPixels++;
                            }

                            /* Set new average pixel value */
                            t_MarkedEncryptedImage.SetPixel(imgY + 1, imgX, avgPixelValue / avgOfNPixels);

                            /**
                             * If we are not in the top-left block, calculate average pixel value
                             * for the top-right pixel.
                             */
                            if (imgY != 0) {
                                avgPixelValue = t_MarkedEncryptedImage.GetPixel(imgY, imgX);
                                avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY + 1, imgX + 1);
                                avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY, imgX + 2);
                                avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY - 1, imgX + 1);
                                t_MarkedEncryptedImage.SetPixel(imgY, imgX + 1, avgPixelValue / 4);
                            }
                        }
                        else {
                            /* Last column */
                            avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY, imgX);
                            avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY + 1, imgX + 1);
                            avgOfNPixels += 2;

                            if (imgY >= 2) {
                                avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY - 1, imgX + 1);
                                avgOfNPixels++;
                            }

                            /* Set new average pixel value */
                            t_MarkedEncryptedImage.SetPixel(imgY, imgX + 1, avgPixelValue / avgOfNPixels);

                            /**
                             * If we are not in the down-right block, calculate average pixel value
                             * for the down-left pixel.
                             */
                            if (imgY != t_MarkedEncryptedImage.GetHeight() - 2) {
                                avgPixelValue = t_MarkedEncryptedImage.GetPixel(imgY, imgX);
                                avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY + 1, imgX + 1);
                                avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY + 2, imgX);
                                avgPixelValue += t_MarkedEncryptedImage.GetPixel(imgY + 1, imgX - 1);
                                t_MarkedEncryptedImage.SetPixel(imgY + 1, imgX, avgPixelValue / 4);
                            }
                        }
                    }
                }
            }
        }

        /**
         * Fourth direct decryption pass.
         * Decrypts remaining pixels.
         */
        for (uint32_t imgY = 2; imgY < t_MarkedEncryptedImage.GetHeight() - 2; imgY += 2) {
            for (uint32_t imgX = 2; imgX < t_MarkedEncryptedImage.GetWidth() - 2; imgX += 2) {
                /* What type of block we are currently looking at? */
                if (t_MarkedEncryptedImage.GetPixel(imgY, imgX) & 1) {
                    uint16_t avgPixelValue = (
                        t_MarkedEncryptedImage.GetPixel(imgY - 1, imgX + 1) +
                        t_MarkedEncryptedImage.GetPixel(imgY + 1, imgX + 1) +
                        t_MarkedEncryptedImage.GetPixel(imgY, imgX) +
                        t_MarkedEncryptedImage.GetPixel(imgY, imgX + 2)
                    ) / 4;

                    t_MarkedEncryptedImage.SetPixel(imgY, imgX + 1, avgPixelValue);

                    avgPixelValue = (
                        t_MarkedEncryptedImage.GetPixel(imgY, imgX) +
                        t_MarkedEncryptedImage.GetPixel(imgY + 2, imgX) +
                        t_MarkedEncryptedImage.GetPixel(imgY + 1, imgX - 1) +
                        t_MarkedEncryptedImage.GetPixel(imgY + 1, imgX + 1)
                    ) / 4;
                    t_MarkedEncryptedImage.SetPixel(imgY + 1, imgX, avgPixelValue);
                }
            }
        }

        /* Added so that the benchmarks module can use this function without writing any files */
        if (t_RecoveredImagePath.size() != 0) {
            t_MarkedEncryptedImage.Save(t_RecoveredImagePath);
        }
    }

    void Extractor::ExtractData(
        BmpImage& t_MarkedEncryptedImage,
        const std::string t_ExtractedDataPath,
        std::vector<uint8_t>& t_DataEmbeddingKey
    )
    {
        std::string userDataBitStream;
        
        ExtractBitStreams(t_MarkedEncryptedImage, t_DataEmbeddingKey, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, userDataBitStream);

        if (t_ExtractedDataPath.size() != 0) {
            BOOST_LOG_TRIVIAL(info) << "Saving " << std::distance(userDataBitStream.begin(), userDataBitStream.end()) << " bits of embedded user-data";
            utils::SaveBitstreamToFile<uint8_t>(t_ExtractedDataPath, userDataBitStream.begin(), userDataBitStream.end());
        }
    }

    void Extractor::RecoverImageAndExract(
        BmpImage& t_MarkedEncryptedImage,
        const std::string t_RecoveredImagePath,
        const std::string t_ExtractedDataPath,
        std::vector<uint8_t>& t_DataEmbeddingKey,
        std::vector<uint8_t>& t_EncryptionKey
    ) 
    {
        /* Get reference to a consts object. */
        Consts& constsRef = Consts::Instance();

        /* Create Huffman-coder object, which will be used to encode RLC sequences */
        Huffman<std::pair<uint16_t, Color16s>, pair_hash> huffmanCoder(consts::c_DefaultNode);

        /* Set default frequencies (found using statistical approach) */
        huffmanCoder.SetFrequencies(consts::huffman::c_DefaultFrequencies);

        std::vector<uint16_t> rlcEncodedBlocksLengths;
        std::string rlcEncodedBitStream;
        std::string lsbEncodedBitStream;
        std::string hashesBitStream;
        std::string lsbsBitStream;
        std::string userDataBitStream;
        ExtractBitStreams(t_MarkedEncryptedImage, t_DataEmbeddingKey, rlcEncodedBlocksLengths, rlcEncodedBitStream, lsbEncodedBitStream, hashesBitStream, lsbsBitStream, userDataBitStream);

        auto rlcCodedLengthsIter = rlcEncodedBlocksLengths.begin();
        auto rlcCodedLengthsSliceStart = rlcEncodedBlocksLengths.begin();
        auto rlcCodedLengthsSliceEnd = rlcEncodedBlocksLengths.begin();
        auto rlcCodedSliceStart = rlcEncodedBitStream.begin();
        auto rlcCodedSliceEnd = rlcEncodedBitStream.begin();

        assert(lsbsBitStream.size() == (t_MarkedEncryptedImage.GetHeight() * t_MarkedEncryptedImage.GetWidth()) / 4);

        for (uint32_t imgY = 0; imgY < t_MarkedEncryptedImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_MarkedEncryptedImage.GetWidth(); imgX += 2) {
                /* What type of block we are currently looking at? */
                if (t_MarkedEncryptedImage.GetPixel(imgY, imgX) & 1) {
                    if (rlcCodedLengthsIter >= rlcEncodedBlocksLengths.end()) {
                        throw std::invalid_argument("Error, while decompressing RLC-encoded blocks! The lengths iterator points beyond the vector end.");
                    }

                    uint16_t currentLength = *(rlcCodedLengthsIter++);
                    
                }
                else {

                }
            }
        }

        if (t_ExtractedDataPath.size() != 0) {
            BOOST_LOG_TRIVIAL(info) << "Saving " << std::distance(userDataBitStream.begin(), userDataBitStream.end()) << " bits of embedded user-data";
            utils::SaveBitstreamToFile<uint8_t>(t_ExtractedDataPath, userDataBitStream.begin(), userDataBitStream.end());
        }

        /* Added so that the benchmarks module can use this function without writing any files */
        if (t_RecoveredImagePath.size() != 0) {
            t_MarkedEncryptedImage.Save(t_RecoveredImagePath);
        }
    }

    void Extractor::ExtractBitStreams(
        const BmpImage& t_MarkedEncryptedImage,
        std::vector<uint8_t>& t_DataEmbeddingKey,
        std::optional<std::reference_wrapper<std::vector<uint16_t>>> t_RlcEncodedLengths,
        std::optional<std::reference_wrapper<std::string>> t_RlcEncodedBitStream,
        std::optional<std::reference_wrapper<std::string>> t_LsbEncodedBitStream,
        std::optional<std::reference_wrapper<std::string>> t_HashesBitStream,
        std::optional<std::reference_wrapper<std::string>> t_LsbsBitStream,
        std::optional<std::reference_wrapper<std::string>> t_UserDataBitStream
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
        uint32_t numberOfBitsFromLsbEncodeGroups{ 0 };

        /* Divide image into blocks, and calculate number of omega one blocks */
        for (uint32_t imgY = 0; imgY < t_MarkedEncryptedImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_MarkedEncryptedImage.GetWidth(); imgX += 2) {
                /* What type of block we are currently looking at? */
                if (t_MarkedEncryptedImage.GetPixel(imgY, imgX) & 1) {
                    /* RLC-encoded block */
                    omegaOneBlocks++;
                }
            }
        }

        /* Find value of xi, to calculate total number of bits used for data embedding. */
        uint32_t xi = utils::math::Floor((float)(totalBlocks - omegaOneBlocks) / (float)constsRef.GetLambda());
        uint32_t totalBitsFromLsbEncodedGroups = (xi * constsRef.GetLambda() * (4 * constsRef.GetLsbLayers() - 1));

        for (uint32_t imgY = 0; imgY < t_MarkedEncryptedImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_MarkedEncryptedImage.GetWidth(); imgX += 2) {
                /* What type of block we are currently looking at? */
                if (t_MarkedEncryptedImage.GetPixel(imgY, imgX) & 1) {
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
                            for (int32_t bitPos = constsRef.GetLsbLayers() - 1; bitPos >= (yAdd == 0 && xAdd == 0) ? 1 : 0; bitPos--) {
                                if (numberOfBitsFromLsbEncodeGroups >= totalBitsFromLsbEncodedGroups) {
                                    break;
                                }
                                else {
                                    extractedBitStream += utils::math::GetNthBit(curPixel, bitPos) ? '1' : '0';
                                    numberOfBitsFromLsbEncodeGroups++;
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

        /**
         * Now, when we have this bitstream: {\Re || C || \Lambda || H || F || S }.
         * Start a parsing operation.
         */

         /* Total size of C bitstream */
        uint32_t rlcEncodedBitstreamSize{ 0 };

        auto sliceBegin = extractedBitStream.begin();
        auto sliceEnd = extractedBitStream.begin();
        utils::Advance(sliceEnd, extractedBitStream.end(), constsRef.GetRlcEncodedMaxSize());
        rlcEncodedBitstreamSize += boost::dynamic_bitset<>(std::string(sliceBegin, sliceEnd)).to_ulong();

        /* Sum all of the rlc-encoded block lengths */
        for (uint32_t currentRlcCodedBlock = 0; currentRlcCodedBlock < omegaOneBlocks - 1; currentRlcCodedBlock++) {
            uint16_t currRlcEncodedBlockSize = boost::dynamic_bitset<>(
                std::string(
                    utils::Advance(sliceBegin, extractedBitStream.end(), constsRef.GetRlcEncodedMaxSize()),
                    utils::Advance(sliceEnd, extractedBitStream.end(), constsRef.GetRlcEncodedMaxSize())
                )
            ).to_ulong();
            rlcEncodedBitstreamSize += currRlcEncodedBlockSize;

            if (t_RlcEncodedBitStream) {
                (*t_RlcEncodedBitStream).get().push_back(currRlcEncodedBlockSize);
            }
        }

        utils::Advance(sliceBegin, extractedBitStream.end(), constsRef.GetRlcEncodedMaxSize());
        utils::Advance(sliceEnd, extractedBitStream.end(), rlcEncodedBitstreamSize + xi * (constsRef.GetLambda() * (4 * constsRef.GetLsbLayers() - 1) - constsRef.GetAlpha()));
        if (t_LsbEncodedBitStream) {
            (*t_LsbEncodedBitStream).get() = std::string(sliceBegin, sliceEnd);
        }

        utils::Advance(sliceBegin, extractedBitStream.end(), rlcEncodedBitstreamSize + xi * (constsRef.GetLambda() * (4 * constsRef.GetLsbLayers() - 1) - constsRef.GetAlpha()));
        utils::Advance(sliceEnd, extractedBitStream.end(), xi * constsRef.GetLsbHashSize());
        if (t_HashesBitStream) {
            (*t_HashesBitStream).get() = std::string(sliceBegin, sliceEnd);
        }

        utils::Advance(sliceBegin, extractedBitStream.end(), xi* constsRef.GetLsbHashSize());
        utils::Advance(sliceEnd, extractedBitStream.end(), totalBlocks);
        if (t_LsbsBitStream) {
            (*t_LsbsBitStream).get() = std::string(sliceBegin, sliceEnd);
        }

        utils::Advance(sliceBegin, extractedBitStream.end(), totalBlocks);
        utils::Advance(sliceEnd, extractedBitStream.end(), std::distance(sliceEnd, extractedBitStream.end()));
        if (t_UserDataBitStream) {
            (*t_UserDataBitStream).get() = std::string(sliceBegin, sliceEnd);
        }
    }
}
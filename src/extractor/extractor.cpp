#include "extractor/extractor.h"

#include "embedder/huffman.h"
#include "embedder/compressor.h"
#include "embedder/embedder.h"

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
        
        ExtractBitStreams(t_MarkedEncryptedImage, t_DataEmbeddingKey, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, userDataBitStream, std::nullopt);

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

        /* Total number of 2x2 pixels blocks. In the article it's referred as L. */
        uint32_t totalBlocks{
            static_cast<std::size_t>(t_MarkedEncryptedImage.GetHeight()) * static_cast<std::size_t>(t_MarkedEncryptedImage.GetWidth()) / 4
        };

        /* Number of blocks encoded using RLC-based algorithm. In the article it's referred as R. */
        uint32_t omegaOneBlocks{ 0 };

        /* Create Huffman-coder object, which will be used to encode RLC sequences */
        Huffman<std::pair<uint16_t, Color16s>, pair_hash> huffmanCoder(consts::c_DefaultNode);

        /* Set default frequencies (found using statistical approach) */
        huffmanCoder.SetFrequencies(consts::huffman::c_DefaultFrequencies);

        std::vector<uint16_t> rlcCompressedBlocksLengths;
        std::string rlcCompressedBitStream;
        std::vector<std::string> lsbCompressedGroups;
        std::vector<std::string> groupsHashes;
        std::string lsbsBitStream;
        std::string userDataBitStream;
        std::vector<bool> binaryLocationMap;
        ExtractBitStreams(t_MarkedEncryptedImage, t_DataEmbeddingKey, rlcCompressedBlocksLengths, rlcCompressedBitStream, lsbCompressedGroups, groupsHashes, lsbsBitStream, userDataBitStream, binaryLocationMap);

        assert(lsbsBitStream.size() == (t_MarkedEncryptedImage.GetHeight() * t_MarkedEncryptedImage.GetWidth()) / 4);
        assert(binaryLocationMap.size() == (t_MarkedEncryptedImage.GetHeight() * t_MarkedEncryptedImage.GetWidth()) / 4);

        if (rlcCompressedBlocksLengths.size() == 0) {
            throw std::invalid_argument("Error, while decompressing RLC-encoded blocks! The vector with RLC-compressed blocks lengths has zero size!");
        }

        /* Iterator that keeps track of the current lsb */
        auto lsbsBitStreamIter = lsbsBitStream.begin();
        /* Iterator that keeps track of current rlc-compressed block size */
        auto rlcCodedLengthsIter = rlcCompressedBlocksLengths.begin();
        /* Slice iterators for rlc-compressed bitstream */
        auto rlcCompressedSliceStart = rlcCompressedBitStream.begin();
        auto rlcCompressedSliceEnd = rlcCompressedBitStream.begin();
        
        /* current key index */
        uint32_t keyCursor{ 0 };
        /* size of the current rlc-compressed block */
        uint32_t currenRlcCompressedSize{ 0 };
        /* Used to keep track of the current block index. */
        uint32_t currBlockIdx{ 0 };

        /* Iterate over all 2x2 px blocks */
        for (uint32_t imgY = 0; imgY < t_MarkedEncryptedImage.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_MarkedEncryptedImage.GetWidth(); imgX += 2) {
                if (lsbsBitStreamIter >= lsbsBitStream.end()) {
                    throw std::invalid_argument("Error, while decompressing RLC-encoded blocks! The lsbs iterator points beyond the bitstream end.");
                }
                
                /* restore original LSB */
                t_MarkedEncryptedImage.SetPixel(
                    imgY, imgX,
                    utils::ClearLastNBits(t_MarkedEncryptedImage.GetPixel(imgY, imgX), 1) | ((*(lsbsBitStreamIter++) == '1') ? 1 : 0)
                );

                /* What type of block we are currently looking at? */
                if (binaryLocationMap.at(currBlockIdx)) {
                    std::string currRlcEncodedSeq;
                    std::vector<Color8u> decompressedColors;

                    if (rlcCodedLengthsIter >= rlcCompressedBlocksLengths.end()) {
                        throw std::invalid_argument("Error, while decompressing RLC-encoded blocks! The lengths iterator points beyond the vector end.");
                    }

                    currenRlcCompressedSize = *(rlcCodedLengthsIter++);

                    assert(rlcCompressedSliceStart == rlcCompressedSliceEnd);
                    
                    /* Decompress current block */
                    decompressedColors = RlcCompressor::Decompress(
                        t_MarkedEncryptedImage.GetPixel(imgY, imgX),
                        std::string(
                            rlcCompressedSliceStart,
                            utils::Advance(rlcCompressedSliceEnd, rlcCompressedBitStream.end(), currenRlcCompressedSize, true)
                        ),
                        huffmanCoder
                    );
                    utils::Advance(rlcCompressedSliceStart, rlcCompressedBitStream.end(), currenRlcCompressedSize, true);

                    /* Decrypt each pixel in the current block to it's original value */
                    t_MarkedEncryptedImage.SetPixel(imgY, imgX, decompressedColors.at(0) ^ t_EncryptionKey[keyCursor % t_EncryptionKey.size()]);
                    t_MarkedEncryptedImage.SetPixel(imgY, imgX + 1, decompressedColors.at(1) ^ t_EncryptionKey[keyCursor % t_EncryptionKey.size()]);
                    t_MarkedEncryptedImage.SetPixel(imgY + 1, imgX, decompressedColors.at(2) ^ t_EncryptionKey[keyCursor % t_EncryptionKey.size()]);
                    t_MarkedEncryptedImage.SetPixel(imgY + 1, imgX + 1, decompressedColors.at(3) ^ t_EncryptionKey[keyCursor % t_EncryptionKey.size()]);
                    
                    omegaOneBlocks++;
                }

                /* Move binary location "pointer" to the next entry. */
                ++currBlockIdx;
                /* Update key cursor value. */
                ++keyCursor;
            }
        }

        /* Next step. Recover lsbs of LSB-compressed blocks */
        uint32_t xi = utils::math::Floor((float)(totalBlocks - omegaOneBlocks) / (float)constsRef.GetLambda());
        Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic> psi(constsRef.GetAlpha(), constsRef.GetGroupRowsCount());

        /* In the article it's referred as Z'. */
        Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic> pseudoRandomMat(constsRef.GetMatrixRows(), constsRef.GetAlpha());
        Embedder::PreparePseudoRandomMatrix(pseudoRandomMat, t_DataEmbeddingKey);
        pseudoRandomMat.transposeInPlace();

        /* Create binary matrix as described in the article */
        psi << pseudoRandomMat, Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>::Identity(constsRef.GetAlpha(), constsRef.GetAlpha());

        /* Vector of group candidates for each i = 1, ..., xi */
        std::vector<std::vector<Eigen::Matrix<uint8_t, 1, Eigen::Dynamic>>> vectorOfGroupCandidates;
        vectorOfGroupCandidates.reserve(xi);

        for (uint32_t currGroupIdx = 0; currGroupIdx < xi; ++currGroupIdx) {
            vectorOfGroupCandidates.emplace_back(std::vector<Eigen::Matrix<uint8_t, 1, Eigen::Dynamic>>());
            Eigen::Matrix<uint8_t, 1, Eigen::Dynamic> rowVector(1, constsRef.GetAlpha());
            
            for (uint32_t currRowVector = 0; currRowVector < std::powl(2, constsRef.GetAlpha()); ++currRowVector) {
                Eigen::Matrix<uint8_t, 1, Eigen::Dynamic> firstPart(1, lsbCompressedGroups.at(currGroupIdx).size() + constsRef.GetAlpha());
                firstPart << utils::ConvertBinaryStringToMatrix(lsbCompressedGroups.at(currGroupIdx)), Eigen::Matrix<uint8_t, 1, Eigen::Dynamic>(1, constsRef.GetAlpha());

                std::string rowVectorStr;
                boost::to_string(boost::dynamic_bitset<>(constsRef.GetAlpha(), currRowVector), rowVectorStr);
                rowVector = utils::ConvertBinaryStringToMatrix(rowVectorStr);

                vectorOfGroupCandidates.back().emplace_back(firstPart + rowVector * psi);
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
        std::optional<std::reference_wrapper<std::vector<uint16_t>>> t_RlcCompressedBlocksLengths,
        std::optional<std::reference_wrapper<std::string>> t_RlcCompressedBitStream,
        std::optional<std::reference_wrapper<std::vector<std::string>>> t_LsbCompressedGroups,
        std::optional<std::reference_wrapper<std::vector<std::string>>> t_GroupHashesBitStream,
        std::optional<std::reference_wrapper<std::string>> t_LsbsBitStream,
        std::optional<std::reference_wrapper<std::string>> t_UserDataBitStream,
        std::optional<std::reference_wrapper<std::vector<bool>>> t_BinaryLocationMap
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

                /* Update bit in binary-location map */
                if (t_BinaryLocationMap) {
                    (*t_BinaryLocationMap).get().push_back(t_MarkedEncryptedImage.GetPixel(imgY, imgX) & 1);
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
        uint32_t rlcCompressedBitStreamSize{ 0 };

        auto sliceBegin = extractedBitStream.begin();
        auto sliceEnd = extractedBitStream.begin();

        /* Extract lengths information from rlcCompressedBitStream */
        for (uint32_t currentRlcCodedBlock = 0; currentRlcCodedBlock < omegaOneBlocks; currentRlcCodedBlock++) {
            uint16_t currRlcEncodedBlockSize = boost::dynamic_bitset<>(
                std::string(
                    sliceBegin,
                    utils::Advance(sliceEnd, extractedBitStream.end(), constsRef.GetRlcEncodedMaxSize())
                )
            ).to_ulong();
            rlcCompressedBitStreamSize += currRlcEncodedBlockSize;

            utils::Advance(sliceBegin, extractedBitStream.end(), constsRef.GetRlcEncodedMaxSize());

            if (t_RlcCompressedBlocksLengths) {
                (*t_RlcCompressedBlocksLengths).get().push_back(currRlcEncodedBlockSize);
            }
        }

        /* Extract rlc-compressed bitstream */
        utils::Advance(sliceEnd, extractedBitStream.end(), rlcCompressedBitStreamSize);
        if (t_RlcCompressedBitStream) {
            (*t_RlcCompressedBitStream).get() = std::string(sliceBegin, sliceEnd);
            assert(rlcCompressedBitStreamSize == (*t_RlcCompressedBitStream).get().size());
        }

        /* Extract lsb-compressed groups */
        utils::Advance(sliceBegin, extractedBitStream.end(), rlcCompressedBitStreamSize);
        for (uint32_t currentGroup = 0; currentGroup < xi; ++currentGroup) {
            utils::Advance(sliceEnd, extractedBitStream.end(), (constsRef.GetLambda() * (4 * constsRef.GetLsbLayers() - 1) - constsRef.GetAlpha()));

            if (t_LsbCompressedGroups) {
                (*t_LsbCompressedGroups).get().emplace_back(sliceBegin, sliceEnd);
                assert((*t_LsbCompressedGroups).get().back().size() == (constsRef.GetLambda() * (4 * constsRef.GetLsbLayers() - 1) - constsRef.GetAlpha()));
            }

            utils::Advance(sliceBegin, extractedBitStream.end(), (constsRef.GetLambda() * (4 * constsRef.GetLsbLayers() - 1) - constsRef.GetAlpha()));
        }
        assert(xi == (*t_LsbCompressedGroups).get().size());

        /* Extract bitstream with hashes */
        for (uint32_t currentGroup = 0; currentGroup < xi; ++currentGroup) {
            utils::Advance(sliceEnd, extractedBitStream.end(), constsRef.GetLsbHashSize());

            if (t_GroupHashesBitStream) {
                (*t_GroupHashesBitStream).get().emplace_back(sliceBegin, sliceEnd);
                assert(constsRef.GetLsbHashSize() == (*t_GroupHashesBitStream).get().back().size());
            }

            utils::Advance(sliceBegin, extractedBitStream.end(), constsRef.GetLsbHashSize());
        }
        assert(xi == (*t_GroupHashesBitStream).get().size());
        
        /* Extract bitstream with lsbs */
        utils::Advance(sliceEnd, extractedBitStream.end(), totalBlocks);
        if (t_LsbsBitStream) {
            (*t_LsbsBitStream).get() = std::string(sliceBegin, sliceEnd);
            assert(totalBlocks == (*t_LsbsBitStream).get().size());
        }

        /* Extract bitstream with user-data */
        utils::Advance(sliceBegin, extractedBitStream.end(), totalBlocks);
        utils::Advance(sliceEnd, extractedBitStream.end(), std::distance(sliceEnd, extractedBitStream.end()));
        if (t_UserDataBitStream) {
            (*t_UserDataBitStream).get() = std::string(sliceBegin, sliceEnd);
        }
    }
}
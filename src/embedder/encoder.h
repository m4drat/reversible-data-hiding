#pragma once 

#include <string>
#include <vector>

#include "embedder/rlc.h"
#include "embedder/embedder.h"
#include "embedder/huffman.h"

namespace rdh {
    class RlcCompressor {
    public:
        /**
         * @brief Method that encodes given pixel data using RLC encoding + Huffman.
         * @param t_Pixel1 Value of the upper-left pixel
         * @param t_Pixel2 Value of the upper-right pixel
         * @param t_Pixel3 Value of the lower-left pixel
         * @param t_Pixel4 Value of the lower-right pixel
         * @param t_HuffmanCoder Huffman coder object to use
         * @return std::string that represents encoded data in a form of a bitstream
        */
        static std::string Compress(Color8u t_Pixel1, Color8u t_Pixel2, Color8u t_Pixel3, Color8u t_Pixel4, Huffman<std::pair<uint16_t, Color16s>, pair_hash>& t_HuffmanCoder)
        {
            std::vector<std::pair<uint16_t, Color16s>> rlcEncoded;
            std::string encoded;

            /**
             * Difference can be a negative number!
             */
            Color16s deltaM2 = (Color16s)t_Pixel2 - (Color16s)t_Pixel1;
            Color16s deltaM3 = (Color16s)t_Pixel3 - (Color16s)t_Pixel1;
            Color16s deltaM4 = (Color16s)t_Pixel4 - (Color16s)t_Pixel1;

            if (deltaM2 == 0 && deltaM3 == 0 && deltaM4 == 0) {
                rlcEncoded.emplace_back(0, 0);
            }
            else {
                /**
                 * What if we have something like this: 1, 1, 0 -> (0, 1), (0, 1), (0, 0) ???
                 * Is it okay to encode this sequences, as showed in the previous line? (Cause (0, 0) is a special symbol)
                 * Probably this should be okay, because we still can decode this message using information, that in total
                 * we always should have 3 difference values. Thus we can find out, that we should append one more block
                 * to the end with this content: (vlc, vli) = (0, 0).
                */
                rlcEncoded = std::move(RLC::RlcEncode<uint16_t, Color16s>({ deltaM2, deltaM3, deltaM4 }, 0));

                // If the last block is (0, 0), throw it away, because
                // we don't want to encode this element later using Huffman coding.
                if (rlcEncoded.back() == std::pair<uint16_t, Color16s>(0, 0)) {
                    rlcEncoded.pop_back();
                }

                assert((rlcEncoded.back() != std::pair<uint16_t, Color16s>(0, 0)));
            }

            /**
             * Second step. Encode each rlc-encoded block using Huffman coding.
             */
            if (rlcEncoded.size() == 1 &&
                rlcEncoded.at(0) == std::pair<uint16_t, Color16s>(0, 0)) {
                // Special case. All differences are equal to zero.
                // In this scenario, huffman keyword length is 0.
                encoded = "";
            }
            else {
                // We know, that the last element will always be non-zero
                // @sa EncodedBlock::EncodedBlock
                encoded = t_HuffmanCoder.Encode(rlcEncoded);
                assert(encoded.size() > 0);
            }

            return std::move(encoded);
        }

        static std::vector<Color8u> Decompress(Color8u t_Pixel1, const std::string& t_RlcEncoded, Huffman<std::pair<uint16_t, Color16s>, pair_hash>& t_HuffmanCoder)
        {
            return std::vector<Color8u>();
        }
    };
}

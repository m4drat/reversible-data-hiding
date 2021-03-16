#pragma once 

#include <string>
#include <vector>

#include "embedder/rlc.h"

namespace rdh {
    /**
     * @brief Represents 2x2 encoded pixels block.
    */
    class EncodedBlock {
    public:
        EncodedBlock(Color8u t_Pixel1, Color8u t_Pixel2, Color8u t_Pixel3, Color8u t_Pixel4) :
            m_Pixel1{ t_Pixel1 }, m_Pixel2{ t_Pixel2 }, m_Pixel3{ t_Pixel3 }, m_Pixel4{ t_Pixel4 }, m_IsSigmaOne{ false }
        {
            /**
             * Difference can be a negative number!
             */
            Color16s deltaM2 = (Color16s)t_Pixel2 - (Color16s)t_Pixel1;
            Color16s deltaM3 = (Color16s)t_Pixel3 - (Color16s)t_Pixel1;
            Color16s deltaM4 = (Color16s)t_Pixel4 - (Color16s)t_Pixel1;

            if (deltaM2 == 0 && deltaM3 == 0 && deltaM4 == 0) {
                m_RlcEncoded.emplace_back(0, 0);
            }
            else {
                /**
                 * What if we have something like this: 1, 1, 0 -> (0, 1), (0, 1), (0, 0) ???
                 * Is it okay to encode this sequences, as showed on the previous lines? (Cause (0, 0) is a special symbol)
                 * Probably this should be okay, because we still can decode this message using information, that in total
                 * we always should have 3 difference values. Thus we can find out, that we should append one more block 
                 * to the end with this content: (vlc, vli) = (0, 0).
                */
                m_RlcEncoded = std::move(RLC::RlcEncode<uint16_t, Color16s>({ deltaM2, deltaM3, deltaM4 }));

                // If the last block is (0, 0), throw it away, because
                // we don't want to encode this element later using Huffman coding.
                if (m_RlcEncoded.back() == std::pair<uint16_t, Color16s>(0, 0)) {
                    m_RlcEncoded.pop_back();
                }
                assert((m_RlcEncoded.back() != std::pair<uint16_t, Color16s>(0, 0)));
            }
        }

        Color8u& GetPixel1() 
        {
            return m_Pixel1;
        }

        Color8u& GetPixel2()
        {
            return m_Pixel2;
        }

        Color8u& GetPixel3()
        {
            return m_Pixel3;
        }

        Color8u& GetPixel4()
        {
            return m_Pixel4;
        }

        const std::vector<std::pair<uint16_t, Color16s>>& GetRlcEncoded()
        {
            return m_RlcEncoded;
        }

        const bool IsSigmaOne()
        {
            return m_IsSigmaOne;
        }

        void SetIsSigmaOne(bool t_IsSigmaOne)
        {
            m_IsSigmaOne = t_IsSigmaOne;
        }

        void SetEncoded(const std::string& t_Encoded)
        {
            m_Encoded = t_Encoded;
        }

        const std::string& GetEncoded()
        {
            return m_Encoded;
        }

    private:
        /**
         * @brief Represents pixel in the upper left corner of each 2x2 pixels block
        */
        Color8u m_Pixel1;

        /**
         * @brief Represents pixel in the upper right corner of each 2x2 pixels block
        */
        Color8u m_Pixel2;

        /**
        * @brief Represents pixel in the lower left corner of each 2x2 pixels block
        */
        Color8u m_Pixel3;

        /**
        * @brief Represents pixel in the lower right corner of each 2x2 pixels block
        */
        Color8u m_Pixel4;

        /**
         * @brief RlcEncoded block. 
         * @sa RLC
        */
        std::vector <std::pair<uint16_t, Color16s>> m_RlcEncoded;
        
        /**
         * @brief Block representation after Huffman encoding / LSB encoding
        */
        std::string m_Encoded;

        /**
         * @brief Shows, that current block refers to the sigma 1 set. (l_m < T)
        */
        bool m_IsSigmaOne;
    };
}

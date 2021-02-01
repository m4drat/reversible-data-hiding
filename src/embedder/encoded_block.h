#pragma once 

#include <string>
#include <vector>

#include "embedder/rlc.h"

namespace rdh {
    class EncodedBlock {
    public:
        EncodedBlock(Color8 t_MasterPixel, Color8 t_DeltaM1, Color8 t_DeltaM2, Color8 t_DeltaM3) :
            m_MasterPixel{ t_MasterPixel }
        {
            if (t_DeltaM1 == 0 && t_DeltaM2 == 0 && t_DeltaM3 == 0) {
                m_RlcEncoded.emplace_back(0, 0);
            }
            else {
                m_RlcEncoded = std::move(RLC::RlcEncode<uint16_t, Color8>({ t_DeltaM1 , t_DeltaM2, t_DeltaM3 }));
            }
        }

        const Color8& GetMasterPixel() 
        {
            return m_MasterPixel;
        }

        const std::vector<std::pair<uint16_t, Color8>>& GetRlcEncoded()
        {
            return m_RlcEncoded;
        }

        void SetHuffmanEncoded(const std::string& t_HuffmanEncodedRepresentation)
        {
            m_HuffmanCoded = t_HuffmanEncodedRepresentation;
        }

        const std::string& GetHuffmanEncoded()
        {
            return m_HuffmanCoded;
        }

    private:
        /**
         * @brief Represents pixel in the upper left corner of each 2x2 pixels block
        */
        Color8 m_MasterPixel;

        /**
         * @brief RlcEncoded block. 
         * @sa RLC
        */
        std::vector <std::pair<uint16_t, Color8>> m_RlcEncoded;
        
        /**
         * @brief Block representation after Huffman encoding
        */
        std::string m_HuffmanCoded;
    };
}
#pragma once

#include <vector>
#include <unordered_map>
#include <bitset>

namespace rdh {
    template <class T, class Comp>
    class Huffman {
    public:
        Huffman() :
            m_Frequences{}, m_UpdateTree{ true }
        {}

        /**
         * @brief Increments total number of occurrences for symbol t_Symbol
         * @param t_Symbol 
        */
        void UpdateFreqForSymbol(const T& t_Symbol);

        /**
         * @brief Implicitly set ups frequencies table
         * @param m_Frequences 
        */
        void SetFrequences(std::unordered_map<T, uint32_t>&& t_Frequences);

        /**
         * @brief Encodes data passed in the vector t_ToEncode using Huffman coding.
         * @param t_ToEncode[in] vector with data to encode
         * @return Encoded "bit" string
        */
        std::string Encode(const std::vector<T>& t_ToEncode);

        /**
         * @brief Decodes bit string, that represents encoded data 
         * @param t_ToDecode[in] string that represents Huffman-encoded data
         * @param t_Codes[in] mapping between symbol and its Huffman-code
         * @return vector with decoded data
        */
        std::vector<T> Decode(const std::string& t_ToDecode, const std::unordered_map<T, std::string>& t_Codes);
    private:
        /**
         * @brief Rebuild Huffman tree using m_Frequences
        */
        void RebuildTree();

        /**
         * @brief Mapping between symbol and it's frequency
        */
        std::unordered_map<T, uint32_t, Comp> m_Frequences;

        /**
         * @brief mapping between each symbol and its Huffman code
        */
        std::unordered_map<T, std::string, Comp> t_Codes;

        /**
         * @brief Shows if we need to rebuild tree (e.g. we have updated frequencies).
        */
        bool m_UpdateTree;
    };
}

#include "embedder/huffman-impl.h"
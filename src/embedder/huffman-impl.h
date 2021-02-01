#pragma once

#include "embedder/huffman.h"

namespace rdh {
    template <class T, class Comp>
    void Huffman<T, Comp>::UpdateFreqForSymbol(const T& t_Symbol)
    {
        ++m_Frequences[t_Symbol];
        m_UpdateTree = true;
    }

    template <class T, class Comp>
    void Huffman<T, Comp>::SetFrequences(std::unordered_map<T, uint32_t>&& t_Frequences)
    {
        m_Frequences = std::move(t_Frequences);
        m_UpdateTree = true;
    }

    template <class T, class Comp>
    std::string Huffman<T, Comp>::Encode(const std::vector<T>& t_ToEncode)
    {
        return std::string();
    }

    template <class T, class Comp>
    std::vector<T> Huffman<T, Comp>::Decode(const std::string& t_ToDecode, const std::unordered_map<T, std::string>& t_Codes)
    {
        return std::vector<T>();
    }

    template <class T, class Comp>
    void Huffman<T, Comp>::RebuildTree()
    {

    }
}
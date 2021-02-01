#pragma once

#include "embedder/huffman.h"

#include <stdexcept>

namespace rdh {
    template <class T, class Hash>
    void Huffman<T, Hash>::UpdateFreqForSymbol(T t_Symbol)
    {
        ++m_Frequencies[t_Symbol];
        m_UpdateTree = true;
    }

    template <class T, class Hash>
    void Huffman<T, Hash>::SetFrequencies(std::unordered_map<T, uint32_t, Hash>&& t_Frequencies)
    {
        m_Frequencies = std::move(t_Frequencies);
        m_UpdateTree = true;
    }

    template <class T, class Hash>
    std::string Huffman<T, Hash>::Encode(const std::vector<T>& t_ToEncode)
    {
        std::string encoded = "";

        if (m_Frequencies.size() == 0) {
            throw std::logic_error("m_Frequencies should be initialized first!");
        }

        if (m_UpdateTree) {
            RebuildTree();
        }

        BuildCodesTable(m_HuffmanTree.top(), "");

        for (const auto& elem : t_ToEncode) {
            encoded += m_Codes.at(elem);
        }

        return std::move(encoded);
    }

    template <class T, class Hash>
    void Huffman<T, Hash>::BuildCodesTable(std::shared_ptr<HuffmanTreeNode> t_Root, std::string t_CurrentCode) {
        if (t_Root == nullptr) {
            return;
        }

        if (t_Root->IsLeaf()) {
            m_Codes[t_Root->symbol] = (!t_CurrentCode.empty()) ? t_CurrentCode : "1";
        }

        BuildCodesTable(t_Root->left, t_CurrentCode + "0");
        BuildCodesTable(t_Root->right, t_CurrentCode + "1");
    }

    template <class T, class Hash>
    std::vector<T> Huffman<T, Hash>::Decode(const std::string& t_ToDecode, const std::unordered_map<T, std::string>& t_Codes)
    {
        if (m_Frequencies.size() == 0) {
            throw std::logic_error("m_Frequencies should be initialized first!");
        }

        if (m_UpdateTree) {
            RebuildTree();
        }

        return std::vector<T>();
    }

    template <class T, class Hash>
    void Huffman<T, Hash>::RebuildTree()
    {
        if (!m_HuffmanTree.empty()) {
            m_HuffmanTree = std::priority_queue<std::shared_ptr<HuffmanTreeNode>, std::vector<std::shared_ptr<HuffmanTreeNode>>, NodeComparator>();
        }

        for (const auto& [symbol, freq] : m_Frequencies) {
            m_HuffmanTree.push(std::make_shared<HuffmanTreeNode>(symbol, freq, nullptr, nullptr));
        }
    
        while (m_HuffmanTree.size() != 1)
        {
            std::shared_ptr<HuffmanTreeNode> left = m_HuffmanTree.top();
            m_HuffmanTree.pop();

            std::shared_ptr<HuffmanTreeNode> right = m_HuffmanTree.top();
            m_HuffmanTree.pop();

            uint32_t freqSum = left->freq + right->freq;
            m_HuffmanTree.push(std::make_shared<HuffmanTreeNode>(m_DefaultNode, freqSum, left, right));
        }

        m_UpdateTree = false;
    }
}
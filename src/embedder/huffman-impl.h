#pragma once

#include "embedder/huffman.h"
#include "types.h"

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
    const std::unordered_map<T, std::string, Hash>& Huffman<T, Hash>::GetCodesTable()
    {
        /**
         * Check if frequencies map or m_Codes are initialized.
         */
        if (m_Frequencies.empty()) {
            throw std::logic_error("m_Frequencies should be initialized first!");
        }
        
        /**
         * If something was changed. We need to rebuild tree, before calculating actual codes.
         */
        if (m_UpdateTree || m_Codes.empty()) {
            RebuildTree();
            BuildCodesTable(m_HuffmanTree.top(), "");
        }

        return m_Codes;
    }

    template <class T, class Hash>
    std::string Huffman<T, Hash>::Encode(const std::vector<T>& t_ToEncode)
    {
        std::string encoded = "";
        encoded.reserve(t_ToEncode.size());

        if (m_Frequencies.empty()) {
            throw std::logic_error("m_Frequencies should be initialized first!");
        }

        // If we need to rebuild tree, we also need to update 
        // corresponding Huffman codes.
        if (m_UpdateTree) {
            RebuildTree();
            BuildCodesTable(m_HuffmanTree.top(), "");
        }

        for (const auto& elem : t_ToEncode) {
            encoded += m_Codes.at(elem);
        }

        return std::move(encoded);
    }

    template <class T, class Hash>
    void Huffman<T, Hash>::BuildCodesTable(std::shared_ptr<HuffmanTreeNode> t_Root, std::string t_CurrentCode) 
    {
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
    std::vector<T> Huffman<T, Hash>::Decode(const std::string& t_ToDecode)
    {
        std::vector<T> decoded;
        decoded.reserve(t_ToDecode.size());

        if (m_Frequencies.empty()) {
            throw std::logic_error("m_Frequencies should be initialized first!");
        }

        if (m_UpdateTree) {
            RebuildTree();
        }

        int32_t idx = -1;
        while (idx < static_cast<int32_t>(t_ToDecode.size()) - 1) {
            decoded.push_back(RestoreOriginalSymbol(m_HuffmanTree.top(), idx, t_ToDecode));
        }

        return decoded;
    }

    template <class T, class Hash>
    T Huffman<T, Hash>::RestoreOriginalSymbol(std::shared_ptr<HuffmanTreeNode> t_Root, int32_t& t_CurrIdx, const std::string& t_ToDecode)
    {
        if (t_Root == nullptr) {
            return m_DefaultNode;
        }

        if (t_Root->IsLeaf()) {
            return t_Root->symbol;
        }

        ++t_CurrIdx;

        if (t_ToDecode[t_CurrIdx] == '0') {
            return RestoreOriginalSymbol(t_Root->left, t_CurrIdx, t_ToDecode);
        }
        else {
            return RestoreOriginalSymbol(t_Root->right, t_CurrIdx, t_ToDecode);
        }
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

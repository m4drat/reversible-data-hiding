#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <queue>
#include <functional>

namespace rdh {
    template <class T, class Hash = std::hash<T>>
    class Huffman {
    public:
        Huffman(T t_DefaultNode) :
            m_Frequencies{}, m_UpdateTree{ true }, m_DefaultNode{ t_DefaultNode }
        {}

        /**
         * @brief Increments total number of occurrences for symbol t_Symbol
         * @param t_Symbol 
        */
        void UpdateFreqForSymbol(T t_Symbol);

        /**
         * @brief Implicitly set ups frequencies table
         * @param m_Frequencies 
        */
        void SetFrequencies(std::unordered_map<T, uint32_t, Hash>&& t_Frequencies);

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
         * @brief Represent Huffman tree node
        */
        class HuffmanTreeNode {
        public:
            T symbol;
            uint32_t freq;
            std::shared_ptr<HuffmanTreeNode> left;
            std::shared_ptr<HuffmanTreeNode> right;

            HuffmanTreeNode(T t_Symbol, uint32_t t_Freq, std::shared_ptr<HuffmanTreeNode> t_Left, std::shared_ptr<HuffmanTreeNode> t_Right) :
                symbol{ t_Symbol }, freq{ t_Freq }, left{ t_Left }, right{ t_Right }
            {}

            bool IsLeaf() {
                return left == nullptr && right == nullptr;
            }
        };

        struct NodeComparator {
            bool operator()(std::shared_ptr<HuffmanTreeNode> t_Left, std::shared_ptr<HuffmanTreeNode> t_Right)
            {
                return t_Left->freq > t_Right->freq;
            }
        };

        /**
         * @brief Rebuild Huffman tree from scratch using m_Frequencies
        */
        void RebuildTree();

        void BuildCodesTable(std::shared_ptr<HuffmanTreeNode> t_Root, std::string t_CurrentCode);

        /**
         * @brief Mapping between symbol and it's frequency
        */
        std::unordered_map<T, uint32_t, Hash> m_Frequencies;

        /**
         * @brief mapping between each symbol and its Huffman code
        */
        std::unordered_map<T, std::string, Hash> m_Codes;

        /**
         * @brief Represents Huffman tree
        */
        std::priority_queue<std::shared_ptr<HuffmanTreeNode>, std::vector<std::shared_ptr<HuffmanTreeNode>>, NodeComparator> m_HuffmanTree;

        /**
         * @brief Shows if we need to rebuild tree (e.g. we have updated frequencies).
        */
        bool m_UpdateTree;

        T m_DefaultNode;

        //const static std::unordered_map<T, uint32_t, Comp> s_DefaultFrequenciesTable{ { }, { } };
    };
}

#include "embedder/huffman-impl.h"
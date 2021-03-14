#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <queue>
#include <functional>

namespace rdh {
    /**
     * @brief Implements Huffman encoder/decoder for custom data type T
     * @tparam T data type to compress using Huffman coding
     * @tparam Comp default comparator for unordered_map that represents codes, frequencies
    */
    template <class T, class Hash = std::hash<T>>
    class Huffman {
    public:
        /**
         * @brief Default constructor
         * @param t_DefaultNode default node object to use while merging nodes inside Huffman tree
        */
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
        void SetFrequencies(const std::unordered_map<T, uint32_t, Hash>& t_Frequencies);

        /**
         * @brief Returns const reference to frequencies map
         * @return std::unordered_map<T, uint32_t, Hash>&
        */
        const std::unordered_map<T, uint32_t, Hash>& GetFrequencies();

        /**
         * @brief Returns unordered_map, where key is a user-defined symbol, and value is a corresponding
         * Huffman code.
         * @return unordered_map 
        */
        const std::unordered_map<T, std::string, Hash>& GetCodesTable();

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
        std::vector<T> Decode(const std::string& t_ToDecode);
    private:
        /**
         * @brief Represent Huffman tree node
        */
        class HuffmanTreeNode {
        public:
            /**
             * @brief symbol associated with the current node
            */
            T symbol;

            /**
             * @brief Frequency of this exact node
            */
            uint32_t freq;

            /**
             * @brief Left node pointer
            */
            std::shared_ptr<HuffmanTreeNode> left;
            
            /**
             * @brief Right node pointer
            */
            std::shared_ptr<HuffmanTreeNode> right;

            /**
             * @brief Default constructor for a tree node
             * @param t_Symbol symbol to associate with the current node (can be char from a message, byte, pair, ...)
             * @param t_Freq frequency of a current node
             * @param t_Left left node pointer
             * @param t_Right right node pointer
            */
            HuffmanTreeNode(T t_Symbol, uint32_t t_Freq, std::shared_ptr<HuffmanTreeNode> t_Left, std::shared_ptr<HuffmanTreeNode> t_Right) :
                symbol{ t_Symbol }, freq{ t_Freq }, left{ t_Left }, right{ t_Right }
            {}

            /**
             * @brief Checks if current node is a leaf or not
             * @return true, if current node doesn't have descendants
            */
            bool IsLeaf() {
                return left == nullptr && right == nullptr;
            }
        };

        /**
         * @brief Custom comparator to use with priority_queue, which is used to build Huffman tree
        */
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

        /**
         * @brief Helper method, to build map with the correspondence between the symbol and Huffman code recursively
         * @param t_Root tree root, to start building map from
         * @param t_CurrentCode current Huffman code value
        */
        void BuildCodesTable(std::shared_ptr<HuffmanTreeNode> t_Root, std::string t_CurrentCode);

        /**
         * @brief Restores original symbol from tree
         * @return Symbol by its Huffman code
        */
        T RestoreOriginalSymbol(std::shared_ptr<HuffmanTreeNode> t_Root, int32_t& t_CurrIdx, const std::string& t_ToDecode);

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

        /**
         * @brief Default node object. Used while building Huffman tree.
        */
        T m_DefaultNode;

        //const static std::unordered_map<T, uint32_t, Comp> s_DefaultFrequenciesTable{ { }, { } };
    };
}

#include "embedder/huffman-impl.h"

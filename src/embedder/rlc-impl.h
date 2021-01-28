#pragma once

#include "embedder/rlc.h"

namespace rdh {
    template <class T>
    static std::vector<std::pair<uint32_t, T>> RLC::RlcEncode(const std::vector<T>& vec)
    {
        /**
        * Represents vector of intermediate symbols.
        * pair.first  = count of zero elements, before first non-zero element
        * pair.second = value of the first non-zero element
        */
        std::vector<std::pair<uint32_t, T>> irSymbols;

        uint32_t lastNulCount = 0;
        for (uint32_t pos = 0; pos < vec.size(); ++pos) {
            if (vec[pos] == 0) {
                ++lastNulCount;
            }
            else {
                irSymbols.emplace_back(lastNulCount, vec[pos]);
                lastNulCount = 0;
            }
        }

        return std::move(irSymbols);
    }
}
#pragma once

#include <stdexcept>

#include "embedder/rlc.h"

namespace rdh {
    template <class T1, class T2>
    static std::vector<std::pair<T1, T2>> RLC::RlcEncode(const std::vector<T2>& vec)
    {
        /**
        * Represents vector of intermediate symbols.
        * pair.first  = count of zero elements, before first non-zero element
        * pair.second = value of the first non-zero element
        */
        std::vector<std::pair<T1, T2>> irSymbols;

        if (vec.size() == 0) {
            throw std::invalid_argument("Vector to encode cannot have 0 elements!");
        }

        T1 lastNulCount = 0;
        for (uint32_t pos = 0; pos < vec.size(); ++pos) {
            if (vec.at(pos) == 0 && pos != vec.size() - 1) {
                lastNulCount++;
                continue;
            }

            irSymbols.emplace_back(lastNulCount, vec.at(pos));
            lastNulCount = 0;
        }

        return std::move(irSymbols);
    }
}
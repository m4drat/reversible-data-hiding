#pragma once

#include <stdexcept>
#include <algorithm>

#include "embedder/rlc.h"
#include "utils.h"

namespace rdh {
    template <class T1, class T2>
    static std::vector<std::pair<T1, T2>> RLC::RlcEncode(const std::vector<T2>& t_Vec, const T2&& t_NullObject)
    {
        /**
        * Represents vector of intermediate symbols.
        * pair.first  = count of zero elements, before first non-zero element
        * pair.second = value of the first non-zero element
        */
        std::vector<std::pair<T1, T2>> irSymbols;

        if (t_Vec.size() == 0) {
            throw std::invalid_argument("Vector to encode cannot have 0 elements!");
        }

        T1 lastNulCount = 0;
        for (uint32_t pos = 0; pos < t_Vec.size(); ++pos) {
            if (t_Vec.at(pos) == t_NullObject && pos != t_Vec.size() - 1) {
                lastNulCount++;
                continue;
            }

            irSymbols.emplace_back(lastNulCount, t_Vec.at(pos));
            lastNulCount = 0;
        }

        return std::move(irSymbols);
    }

    template <class T1, class T2>
    static std::vector<T2> RLC::RlcDecode(const std::vector<std::pair<T1, T2>>& t_Vec, const T2&& t_NullObject)
    {
        std::vector<T2> originalSequence;

        if (t_Vec.size() == 0) {
            throw std::invalid_argument("Vector to decode cannot have 0 elements!");
        }

        for (auto& pair : t_Vec) {
            originalSequence.resize(originalSequence.size() + pair.first + 1, t_NullObject);
            originalSequence.back() = pair.second;
        }

        return std::move(originalSequence);
    }
}
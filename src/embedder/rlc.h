#pragma once

#include <vector>

namespace rdh {
    /**
     * @brief Implements RLC encoder
    */
    class RLC {
    public:
        /**
         * @brief Encodes vec content using RLC encoding.
         * @tparam T value type
         * @param vec Content to encode
         * @return vector of pairs, where pair.first is count of zero elements, before first non-zero element, 
         * and pair.second is a value of the first non-zero element.
        */
        template <class T>
        static std::vector<std::pair<uint32_t, T>> RlcEncode(const std::vector<T>& vec);
    };
}

// Include implementation
#include "embedder/rlc-impl.h"
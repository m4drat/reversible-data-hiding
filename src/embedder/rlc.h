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
        template <class T1, class T2>
        static std::vector<std::pair<T1, T2>> RlcEncode(const std::vector<T2>& vec);
    };
}

// Include implementation
#include "embedder/rlc-impl.h"
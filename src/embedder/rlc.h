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
         * @tparam T1 type, that represents number of zeroes before first non-zero element.
         * @tparam T2 actual type of the data.
         * @param t_Vec Content to encode.
         * @param t_NullObject What should be considered as a null element.
         * @return vector of pairs, where pair.first is count of zero elements, before first non-zero element, 
         * and pair.second is a value of the first non-zero element.
        */
        template <class T1, class T2>
        static std::vector<std::pair<T1, T2>> RlcEncode(const std::vector<T2>& t_Vec, const  T2&& t_NullObject);
        
        /**
         * @brief Decodes given rlc-encoded sequence.
         * @tparam T1 type, that represents number of zeroes before first non-zero element.
         * @tparam T2 actual type of the data.
         * @param t_Vec vector of pairs, that represents rlc-encoded sequence.
         * @param t_NullObject What should be considered as a null element.
         * @return vector with original sequence.
        */
        template <class T1, class T2>
        static std::vector<T2> RlcDecode(const std::vector<std::pair<T1, T2>>& t_Vec, const T2&& t_NullObject);
    };
}

// Include implementation
#include "embedder/rlc-impl.h"
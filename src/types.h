#pragma once

#include <cstdint>
#include <unordered_map>

#include "boost/container_hash/hash.hpp"

namespace rdh {
    using Color8 = uint8_t;
    using Color8s = int8_t;
    using Color16s = int16_t;

    struct pair_hash {
        template <class T1, class T2>
        std::size_t operator () (const std::pair<T1, T2>& p) const {
            std::size_t seed = 0;
            boost::hash_combine(seed, p.first);
            boost::hash_combine(seed, p.second);
            return seed;
        }
    };

    template <class T1, class T2, class T3>
    using PairsMap = std::unordered_map<std::pair<T1, T2>, T3, pair_hash>;
}

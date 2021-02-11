#pragma once

#include "types.h"

namespace rdh {
    namespace consts {
        namespace huffman {
            const static std::unordered_map<std::pair<uint16_t, Color8>, std::string, pair_hash> c_DefaultCodesPairs{
                { std::pair<uint16_t, Color8>(0, 209), "0001110000"}
            };
        }
    }
}
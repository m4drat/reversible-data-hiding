#pragma once

namespace rdh {
    namespace utils {
        template<typename T>
        std::vector<T> Flatten(const std::vector<std::vector<T>>& t_Orig)
        {
            std::vector<T> ret;
            for (const auto& v : t_Orig)
                ret.insert(ret.end(), v.begin(), v.end());
            return ret;
        }
    }
}

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

        template<typename T>
        std::vector<T> HexToBytes(const std::string& t_Hex) {
            if (t_Hex.length() % 2 != 0) {
                throw std::invalid_argument("Hex string length should be multiple of 2!");
            }

            std::vector<T> bytes;

            for (uint32_t i = 0; i < t_Hex.length(); i += 2) {
                std::string byteStr = t_Hex.substr(i, 2);
                if (byteStr.find_first_not_of("0123456789ABCDEFabcdef") != std::string::npos) {
                    throw std::invalid_argument("Incorrect hex string!");
                }
                T byte = static_cast<T>(strtol(byteStr.c_str(), NULL, 16));
                bytes.push_back(byte);
            }

            return bytes;
        }
    }
}

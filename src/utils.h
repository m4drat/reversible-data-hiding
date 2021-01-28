#pragma once

#include <fstream>

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

        template<typename T>
        std::vector<T> LoadFileData(const std::string& t_Filename) {
            // open the file:
            std::ifstream file(t_Filename, std::ios::binary);

            if (!file.is_open()) {
                throw std::runtime_error("Specified file " + t_Filename + " doesn't exist!");
            }

            // Stop eating new lines in binary mode!!!
            file.unsetf(std::ios::skipws);

            // get its size:
            std::streampos fileSize;

            file.seekg(0, std::ios::end);
            fileSize = file.tellg();
            file.seekg(0, std::ios::beg);

            // reserve capacity
            std::vector<T> bytes;
            bytes.reserve(fileSize);

            // read the data:
            bytes.insert(bytes.begin(),
                std::istream_iterator<T>(file),
                std::istream_iterator<T>());

            return std::move(bytes);
        }
    }
}

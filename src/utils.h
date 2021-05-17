#pragma once

#include <fstream>
#include <random>
#include <iterator>
#include <bitset>
#include <cmath>

#include <boost/compute/detail/sha1.hpp>

namespace rdh {
    namespace utils {
        namespace math {
            constexpr uint32_t Floor(float t_Num)
            {
                const uint32_t i = static_cast<uint32_t>(t_Num);
                return t_Num < t_Num ? i - 1 : i;
            }

            constexpr int32_t Ceil(float t_Num)
            {
                return (static_cast<float>(static_cast<int32_t>(t_Num)) == t_Num)
                    ? static_cast<int32_t>(t_Num)
                    : static_cast<int32_t>(t_Num) + ((t_Num > 0) ? 1 : 0);
            }

            constexpr uint32_t FloorLog2(uint32_t t_Num)
            {
                return t_Num == 1 ? 0 : 1 + FloorLog2(t_Num >> 1);
            }

            constexpr uint32_t CeilLog2(uint32_t t_Num)
            {
                return t_Num == 1 ? 0 : FloorLog2(t_Num - 1) + 1;
            }

            constexpr uint8_t GetNthBit(uint32_t t_Num, uint8_t t_NthBit)
            {
                return (t_Num >> t_NthBit) & 1;
            }
        }

        /**
         * @brief Calculates SHA1 hash for provided t_Data.
         * @tparam T any type of collection, that provides data and size functions.
         * @param t_Data data to calculate hash from.
         * @param t_Hash calculated hash.
         */
        template<typename T>
        void CalculateSHA1(const T& t_Data, std::array<uint32_t, 5>& t_Hash)
        {
            boost::uuids::detail::sha1 sha1;
            sha1.process_bytes(t_Data.data(), t_Data.size());
            sha1.get_digest(reinterpret_cast<uint32_t(&)[5]>(t_Hash[0]));
        }

        /**
         * @brief Shuffles user given collection using Fisher-Yates algorithm.
         * @tparam T type of collection. The collection should provide rbegin, rend, size functions.
         * @param t_Seq seq to seed mt19937.
         * @param t_Collection collection to shuffle in-place.
         */
        template<typename T>
        void ShuffleFisherYates(const std::seed_seq& t_Seq, T& t_Collection)
        {
            std::mt19937 mt(t_Seq);
            uint32_t currentIndexCounter = t_Collection.size();

            for (auto iter = t_Collection.rbegin(); iter != t_Collection.rend();
                ++iter, --currentIndexCounter)
            {
                std::uniform_int_distribution<> dis(0, currentIndexCounter);
                const uint32_t randomIndex = dis(mt);

                if (*iter != t_Collection.at(randomIndex))
                {
                    std::swap(t_Collection.at(randomIndex), *iter);
                }
            }
        }

        /**
         * @brief Reverts permutation previously created by calling ShuffleFisherYates.
         * To do so, provide same seed_seq, and permutated collection.
         * @tparam T type of collection. The collection should provide rbegin, rend, size functions.
         * @param t_Seq seq to seed mt19937.
         * @param t_Collection collection to unshuffle.
         */
        template<typename T>
        void DeshuffleFisherYates(const std::seed_seq& t_Seq, T& t_Collection)
        {
            std::mt19937 mt(t_Seq);
            std::vector<uint32_t> randomIndexes;
            randomIndexes.reserve(t_Collection.size());

            for (uint32_t currentIndexCounter = t_Collection.size(); currentIndexCounter > 0; --currentIndexCounter)
            {
                std::uniform_int_distribution<> dis(0, currentIndexCounter);
                randomIndexes.emplace_back(dis(mt));
            }

            auto vecIter = randomIndexes.rbegin();
            for (auto iter = t_Collection.begin(); iter != t_Collection.end() && vecIter != randomIndexes.rend();
                ++iter, ++vecIter)
            {
                if (*iter != t_Collection.at(*vecIter))
                {
                    std::swap(t_Collection.at(*vecIter), *iter);
                }
            }
        }

        template<typename T>
        std::vector<T> Flatten(const std::vector<std::vector<T>>& t_Orig)
        {
            std::vector<T> ret;
            for (const auto& v : t_Orig)
                ret.insert(ret.end(), v.begin(), v.end());
            return ret;
        }

        template<typename T>
        std::vector<T> HexToBytes(const std::string& t_Hex)
        {
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

        /**
         * @brief Converts given vector of data (that can be casted to uint8_t) to a "binary" string.
         * @tparam T data that can be casted to uint8_t
         * @param t_Bytes vector of data to convert
         * @return "binary" representation of a given data
        */
        template<typename T>
        std::string BytesToBinaryString(const std::vector<T>& t_Bytes)
        {
            std::string binaryRepr{ "" };
            binaryRepr.reserve(t_Bytes.size() * 8);

            for (auto& byte : t_Bytes) {
                binaryRepr.append(std::bitset<8>(static_cast<uint8_t>(byte)).to_string());
            }

            return std::move(binaryRepr);
        }

        /**
         * @brief Converts given string to a byte. If string size is less than 8,
         * prepends it with zeroes.
         * @param t_BinStr string in "binary" form (consists of '0' and '1').
         * @return uint8_t decoded byte
        */
        template <class T>
        uint8_t BinaryStringToByte(const T& t_BinStr)
        {
            if (t_BinStr.length() > 8) {
                throw std::invalid_argument("Binary string length can't be bigger than 8!");
            }

            return static_cast<uint8_t>(std::bitset<8>(t_BinStr).to_ulong());
        }

        template <class Iter, class Incr>
        Iter& Advance(Iter& t_Curr, const Iter& t_End, Incr t_N)
        {
            std::size_t remaining(std::distance(t_Curr, t_End));

            if (remaining < t_N)
            {
                t_N = remaining;
            }

            std::advance(t_Curr, t_N);

            return t_Curr;
        }

        constexpr uint8_t ClearLastNBits(uint8_t t_Num, uint8_t t_Nbits)
        {
            return ~((1 << t_Nbits) - 1) & t_Num;
        }

        template<typename T>
        std::vector<T> LoadFileData(const std::string& t_Filename)
        {
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

        template<typename T>
        void SaveDataToFileData(const std::string& t_Filename, const T& t_Data)
        {
            // open the file:
            std::ofstream outFile(t_Filename, std::ios::out | std::ios::binary);

            if (!outFile.is_open()) {
                throw std::runtime_error("Error, while opening file: " + t_Filename);
            }

            outFile.write((char*)t_Data.data(), t_Data.size());
        }

        template<typename T>
        void SaveBitstreamToFile(const std::string& t_Filename, const std::string::iterator& t_SliceBegin, const std::string::iterator& t_SliceEnd)
        {
            std::vector<T> dataToWrite;
            dataToWrite.reserve(std::abs(std::distance(t_SliceBegin, t_SliceEnd)) / 8);

            std::string::iterator sliceBegin = t_SliceBegin;
            std::string::iterator sliceEnd = t_SliceBegin;

            std::string currByte = std::string(
                sliceBegin,
                Advance(sliceEnd, t_SliceEnd, 8)
            );
            if (currByte.size() < 8) {
                currByte += std::string(8 - currByte.size(), '0');
            }
            dataToWrite.emplace_back(BinaryStringToByte(currByte));

            while (sliceBegin != t_SliceEnd) {
                std::string currByte = std::string(
                    Advance(sliceBegin, t_SliceEnd, 8),
                    Advance(sliceEnd, t_SliceEnd, 8)
                );

                if (sliceBegin == sliceEnd) {
                    break;
                }

                if (currByte.size() < 8) {
                    currByte += std::string(8 - currByte.size(), '0');
                }

                dataToWrite.emplace_back(BinaryStringToByte(currByte));
            }

            SaveDataToFileData(t_Filename, dataToWrite);
        }
    }
}

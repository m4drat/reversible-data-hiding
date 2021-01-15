#pragma once

#include "image_matrix.h"

#include <assert.h>
#include <stdexcept>
#include <limits>

namespace rdh {
    template <typename T>
    ImageMatrix<T>::ImageMatrix()
        : m_Height{ 0 }, m_Width{ 0 }, m_ImageMatrix{ 0, std::vector<T>(0, 0) }
    {}

    template <typename T>
    ImageMatrix<T>::ImageMatrix(std::initializer_list<std::initializer_list<T>> t_ImageMatrix)
    {
        if (t_ImageMatrix.size() == 0) {
            throw std::invalid_argument("Initializer list cannot be empty!");
        }

        // Should never happen
        assert(t_ImageMatrix.size() <= std::numeric_limits<uint32_t>::max());

        uint32_t rowIdx = 0;
        uint32_t currWidth = t_ImageMatrix.begin()->size();

        m_Height = t_ImageMatrix.size();
        m_Width = currWidth;

        m_ImageMatrix = { m_Height, std::vector<T>(m_Width, 0x0) };

        for (auto& row : t_ImageMatrix) {
            if (currWidth != std::distance(row.begin(), row.end())) {
                throw std::invalid_argument("All rows in the initializer list must be the same width!");
            }

            std::copy(row.begin(), row.end(), m_ImageMatrix[rowIdx++].begin());
        }
    }

    template <typename T>
    ImageMatrix<T>::ImageMatrix(uint32_t t_Height, uint32_t t_Width, T t_FillColor)
        : m_Height{ t_Height }, m_Width{ t_Width },
        m_ImageMatrix{ t_Height, std::vector<T>(t_Width, t_FillColor) }
    {}

    template <typename T>
    ImageMatrix<T>::ImageMatrix(ImageMatrix&& t_Other) noexcept
    {
        m_ImageMatrix = std::move(t_Other.m_ImageMatrix);

        m_Height = t_Other.m_Height;
        m_Width = t_Other.m_Width;

        t_Other.m_Height = 0;
        t_Other.m_Width = 0;
    }

    template <typename T>
    ImageMatrix<T>& ImageMatrix<T>::operator=(ImageMatrix&& t_Other) noexcept
    {
        if (this != &t_Other)
        {
            m_ImageMatrix.clear();
            m_ImageMatrix.resize(0);

            m_ImageMatrix = std::move(t_Other.m_ImageMatrix);

            m_Height = t_Other.m_Height;
            m_Width = t_Other.m_Width;

            t_Other.m_Height = 0;
            t_Other.m_Width = 0;
        }

        return *this;
    }

    template <typename T>
    ImageMatrix<T>& ImageMatrix<T>::SetPixel(uint32_t t_Y, uint32_t t_X, T t_NewPixelValue)
    {
        m_ImageMatrix.at(t_Y).at(t_X) = t_NewPixelValue;
        return *this;
    }

    template <typename T>
    T ImageMatrix<T>::GetPixel(uint32_t t_Y, uint32_t t_X)
    {
        return m_ImageMatrix.at(t_Y).at(t_X);
    }

    template <typename T>
    ImageMatrix<T> ImageMatrix<T>::Slice(uint32_t t_YStart, uint32_t t_YEnd, uint32_t t_XStart, uint32_t t_XEnd)
    {
        if (t_XEnd - t_XStart < 0)
        {
            throw std::invalid_argument("t_XEnd - t_XStart should be positive value");
        }

        if (t_YEnd - t_YStart < 0)
        {
            throw std::invalid_argument("t_YEnd - t_YStart should be positive value");
        }

        ImageMatrix slicedMatrix(t_YEnd - t_YStart + 1, t_XEnd - t_XStart + 1, 0x0);

        // Copy matrix pixel-by-pixel
        for (uint32_t y = t_YStart; y <= t_YEnd; ++y) {
            for (uint32_t x = t_XStart; x <= t_XEnd; ++x) {
                slicedMatrix.SetPixel(x, y, GetPixel(x, y));
            }
        }

        return std::move(slicedMatrix);
    }

    template <typename T>
    uint32_t ImageMatrix<T>::GetHeight()
    {
        return m_Height;
    }

    template <typename T>
    uint32_t ImageMatrix<T>::GetWidth()
    {
        return m_Width;
    }

    template <typename T>
    std::vector<T>& ImageMatrix<T>::GetRow(uint32_t t_Y)
    {
        return m_ImageMatrix.at(t_Y);
    }

    template <typename T>
    std::vector<std::vector<T>>& ImageMatrix<T>::GetMatrixRaw()
    {
        return m_ImageMatrix;
    }
}

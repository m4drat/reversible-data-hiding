#include "image_matrix.h"

#include <assert.h>

namespace rdh {
    template <typename T>
    ImageMatrix<T>::ImageMatrix()
        : m_Height{ 0 }, m_Width{ 0 }, m_ImageMatrix{ m_Height, std::vector<T>(m_Width, 0) }
    {}

    template <typename T>
    ImageMatrix<T>::ImageMatrix(uint32_t t_Height, uint32_t t_Width, T t_FillColor)
        : m_Height{ t_Height }, m_Width{ t_Width },
        m_ImageMatrix{ m_Height, std::vector<T>(m_Width, t_FillColor) }
    {}

    template <typename T>
    ImageMatrix<T>& ImageMatrix<T>::SetPixel(uint32_t t_X, uint32_t t_Y, T t_NewPixelValue)
    {
        m_ImageMatrix.at(t_Y).at(t_X) = t_NewPixelValue;
        return *this;
    }

    template <typename T>
    T ImageMatrix<T>::GetPixel(uint32_t t_X, uint32_t t_Y)
    {
        return m_ImageMatrix.at(t_Y).at(t_X);
    }

    template <typename T>
    std::unique_ptr<ImageMatrix<T>> ImageMatrix<T>::Slice(uint32_t t_XStart, uint32_t t_XEnd, uint32_t t_YStart, uint32_t t_YEnd)
    {
        assert(t_XEnd - t_XStart >= 0); // just to make sure...
        assert(t_YEnd - t_YStart >= 0);

        auto slicedMatrix = std::make_unique<ImageMatrix>(t_YEnd - t_YStart + 1, t_XEnd - t_XStart + 1, 0x0);

        // Copy matrix pixel-by-pixel
        for (uint32_t y = t_YStart; y <= t_YEnd; ++y) {
            for (uint32_t x = t_XStart; x <= t_XEnd; ++x) {
                slicedMatrix->SetPixel(x, y, GetPixel(x, y));
            }
        }

        return slicedMatrix;
    }

}
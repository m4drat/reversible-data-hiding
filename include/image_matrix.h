#pragma once

#include <vector>
#include <memory>

#include "types.h"

namespace rdh {
    template <typename T>
    class ImageMatrix {
    public:
        /**
         * @brief Default Constructor
        */
        ImageMatrix();

        /**
         * @brief Creates an empty image matrix of size t_Height and t_Width
         * @param t_Height the height of an image matrix to create
         * @param t_Width the width of an image matrix to create
         * @param t_FillColor color to fill with
        */
        ImageMatrix(uint32_t t_Height, uint32_t t_Width, T t_FillColor);

        /**
         * @brief Construct an image matrix using t_ImageMatrix as a source
         * @param m_ImageMatrix reference to matrix, whose copy will be created
        */
        // ImageMatrix(ImageMatrix<T>& t_ImageMatrix);

        /**
         * @brief Set pixel to a specific value at location (x, y)
         * @param t_X x coordinate of the pixel
         * @param t_Y y coordinate of the pixel
         * @param t_NewPixelValue new pixel value
         * @return reference to current object (allows chaining: im.SetPixel(0, 0, 255).SetPixel(0, 1, 255)...)
        */
        ImageMatrix& SetPixel(uint32_t t_X, uint32_t t_Y, T t_NewPixelValue);

        /**
         * @brief Returns pixel value from location (x, y)
         * @param t_X x coordinate of the pixel
         * @param t_Y y coordinate of the pixel
         * @return pixel value
        */
        T GetPixel(uint32_t t_X, uint32_t t_Y);

        /**
         * @brief Creates a slice of an ImageMatrix.
         * @param t_XStart the pixel to be sliced from (x-axis) (including)
         * @param t_XEnd the pixel to be sliced to (x-axis) (including)
         * @param t_YStart the pixel to be sliced from (y-axis) (including)
         * @param t_YEnd the pixel to be sliced to (y-axis) (including)
         * @return New ImageMatrix
        */
        std::unique_ptr<ImageMatrix> Slice(uint32_t t_XStart, uint32_t t_XEnd, uint32_t t_YStart, uint32_t t_YEnd);

    private:
        /**
         * @brief Represents pixels in a specific image
        */
        std::vector<std::vector<T>> m_ImageMatrix;

        uint32_t m_Height;
        uint32_t m_Width;
    };
}
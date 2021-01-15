#pragma once

#include <vector>
#include <memory>

#include "types.h"

namespace rdh {
    /**
     * @brief Class that represents matrix of a pixels
     * 
     * Image are represented as follows:
     *         X-AXIS
     *       +----------->
     *
     *    +  +---------+
     *   Y|  |         |
     *   -+  |         |
     *   A|  |  Image  |
     *   X|  |         |
     *   I|  |         |
     *   S|  +---------+
     *    v
     *
     * @warning Matrix dimensions should be less than or equal to uint32_t::max()
     * @tparam T template parameter that represents one pixel
    */
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
         * @brief Initializes ImageMatrix using initializer list
         * @param t_ImageMatrix initializer list to initialize with
        */
        ImageMatrix(std::initializer_list<std::initializer_list<T>> t_ImageMatrix);

        /**
         * @brief Construct an image matrix using t_ImageMatrix as a source
         * @param m_ImageMatrix reference to matrix, whose copy will be created
        */
        // ImageMatrix(ImageMatrix<T>& t_ImageMatrix);

       /**
         * @brief Move constructor
         * @param t_Other
        */
        ImageMatrix(ImageMatrix&& t_Other) noexcept;

        /**
         * @brief Moves t_Other into this
         * @param t_Other rvalue reference to an object
         * @return ImageMatrix
        */
        ImageMatrix& operator=(ImageMatrix&& t_Other) noexcept;

        /**
         * @brief Set pixel to a specific value at location (y, x)
         * @param t_Y y coordinate of the pixel
         * @param t_X x coordinate of the pixel
         * @param t_NewPixelValue new pixel value
         * @return reference to current object (allows chaining: im.SetPixel(0, 0, 255).SetPixel(0, 1, 255)...)
        */
        ImageMatrix& SetPixel(uint32_t t_Y, uint32_t t_X, T t_NewPixelValue);

        /**
         * @brief Returns pixel value from location (y, x)
         * @param t_Y y coordinate of the pixel
         * @param t_X x coordinate of the pixel
         * @return pixel value
        */
        T GetPixel(uint32_t t_Y, uint32_t t_X);

        /**
         * @brief Creates a slice of an ImageMatrix.
         * @param t_YStart the pixel to be sliced from (y-axis) (including)
         * @param t_YEnd the pixel to be sliced to (y-axis) (including)
         * @param t_XStart the pixel to be sliced from (x-axis) (including)
         * @param t_XEnd the pixel to be sliced to (x-axis) (including)
         * @return New ImageMatrix
        */
        ImageMatrix<T> Slice(uint32_t t_YStart, uint32_t t_YEnd, uint32_t t_XStart, uint32_t t_XEnd);

        /**
         * @brief Get height of an image matrix.
         * @return height of an image
        */
        uint32_t GetHeight();

        /**
         * @brief Get width of an image matrix.
         * @return width of an image
        */
        uint32_t GetWidth();

        /**
         * @brief Returns reference to a specific row
         * @return std::vector<T> that represents row
        */
        std::vector<T>& GetRow(uint32_t t_Y);

        /**
         * @brief Returns whole 2d array as a reference
         * @return reference to a 2d array, that represents matrix
        */
        std::vector<std::vector<T>>& GetMatrixRaw();
    private:
        /**
         * @brief Represents pixels in a specific image
        */
        std::vector<std::vector<T>> m_ImageMatrix;

        /**
         * @brief Height of a matrix, that represents image
        */
        uint32_t m_Height;
        
        /**
         * @brief Width of a matrix, that represents image
        */
        uint32_t m_Width;
    };
}

// Include implementation, because we are working with templates
#include "image_matrix-impl.h"
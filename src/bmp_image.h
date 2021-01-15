#pragma once

#include <string>

#include "types.h"
#include "image_matrix.h"

namespace rdh {
    class BmpImage {
    public:
        /**
         * @brief Creates an empty image of size t_Height and t_Width
         * @param t_Height the height of an image to create
         * @param t_Width the width of an image to create
         * @param t_FillColor color to fill with
        */
        BmpImage(uint32_t t_Height, uint32_t t_Width, Color8 t_FillColor = 0x00);

        /**
         * @brief Loads and converts an image from bmp file format to form of a matrix
         * @param t_ImagePath image path to open
        */
        BmpImage(const std::string& t_ImagePath);

        /**
         * @brief Construct an image using t_Image as a source
         * @param t_Image image, which copy you want to create
        */
        // BmpImage(const BmpImage& t_Image);

        /**
         * @brief Constructs image using image matrix
         * @param t_ImageMatrix matrix, that represents an image
        */
        // BmpImage(std::vector<std::vector<Color8>>& t_ImageMatrix);

        /**
         * @brief Saves current image in bmp format
         * @param t_ImagePath path to save image to
        */
        void Save(const std::string& t_ImagePath);

        /**
         * @brief Returns an image, constructed from a slice of original image
         * @param t_XStart the pixel to be sliced from (x-axis) (including)
         * @param t_XEnd the pixel to be sliced to (x-axis) (including)
         * @param t_YStart the pixel to be sliced from (y-axis) (including)
         * @param t_YEnd the pixel to be sliced to (y-axis) (including)
         * @return New Image
        */
        BmpImage Slice(uint32_t t_XStart, uint32_t t_XEnd, uint32_t t_YStart, uint32_t t_YEnd);

        /**
         * @brief Display image
        */
        void Show();

    private:
        ImageMatrix<Color8> m_ImageMatrix;
    };
}
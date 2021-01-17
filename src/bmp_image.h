#pragma once

#include <string>
#include <ostream>

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
         * @brief Copies t_BmpImage
         * @param t_BmpImage image to construct from
        */
        BmpImage(const BmpImage& t_BmpImage);

        /**
         * @brief Construct an image using t_Image as a source
         * @param t_Image image, which copy you want to create
        */
        // BmpImage(const BmpImage& t_Image);

        /**
         * @brief Constructs image using ImageMatrix
         * @param t_ImageMatrix matrix, that represents an image
        */
        BmpImage(ImageMatrix<Color8>&& t_ImageMatrix);

        /**
         * @brief Move constructor from other image
         * @param t_Other image to move from
        */
        BmpImage(BmpImage&& t_Other);

        /**
         * @brief Moves image t_Other into current object
         * @param t_Other Object to move
         * @return new BmpImage
        */
        BmpImage& operator=(BmpImage&& t_Other) noexcept;

        /**
         * @brief Saves current image in bmp format
         * @param t_ImagePath path to save image to
        */
        void Save(const std::string& t_ImagePath);

        /**
         * @brief Returns cropped image
         * @param t_YStart the pixel to be sliced from (y-axis) (including)
         * @param t_YEnd the pixel to be sliced to (y-axis) (including)
         * @param t_XStart the pixel to be sliced from (x-axis) (including)
         * @param t_XEnd the pixel to be sliced to (x-axis) (including)
         * @return New Image
        */
        BmpImage Crop(uint32_t t_YStart, uint32_t t_YEnd, uint32_t t_XStart, uint32_t t_XEnd);

        /**
         * @brief Set pixel to a specific value at location (y, x)
         * @param t_Y y coordinate of the pixel
         * @param t_X x coordinate of the pixel
         * @param t_NewPixelValue new pixel value
         * @return reference to current object (allows chaining: im.SetPixel(0, 0, 255).SetPixel(0, 1, 255)...)
        */
        BmpImage& SetPixel(uint32_t t_Y, uint32_t t_X, Color8 t_NewPixelValue);

        /**
         * @brief Returns pixel value from location (y, x)
         * @param t_Y y coordinate of the pixel
         * @param t_X x coordinate of the pixel
         * @return pixel value
        */
        Color8 GetPixel(uint32_t t_Y, uint32_t t_X) const;
        
        /**
         * @brief Returns height of the image
         * @return height of the image
        */
        uint32_t GetHeight() const;
        
        /**
         * @brief Returns width of the image
         * @return width of the image
        */
        uint32_t GetWidth() const;

        /**
         * @brief Return ImageMatrix of a current image
         * @return ImageMatrix
        */
        ImageMatrix<Color8>& GetImageMatrix();

        /**
         * @brief Display image
        */
        void Show() const;

        /**
         * @brief Tries to divides image into at least t_DesiredSubdividedImages. Can fail and divide into fewer parts.
         * @param t_DesiredSubdividedImagesCount Desired amount of subimages to divide into
         * @return std::tuple<uint32_t, uint32_t, uint32_t>, where
         *     std::get<0> - height of subimage
         *     std::get<1> - width of subimage
         *     std::get<2> - total number os subimages
        */
        std::tuple<uint32_t, uint32_t, uint32_t> OptimalSubdivision(uint32_t t_DesiredSubdividedImagesCount) const;

        /**
         * @brief Dumps content of an image in a json-like format
         * @param t_Stream file stream to write to
         * @param t_BmpImage image to dump
         * @return std::ostream&
        */
        friend std::ostream& operator<<(std::ostream& t_Stream, BmpImage& t_BmpImage);

    private:
        /**
         * @brief Image matrix that represents pixels of the image
         */
        ImageMatrix<Color8> m_ImageMatrix;
    };
}
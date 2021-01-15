#include "bmp_image.h"
#include "utils.h"

#include "CImg/CImg.h"
using namespace cimg_library;

namespace rdh {

    BmpImage::BmpImage(uint32_t t_Height, uint32_t t_Width, Color8 t_FillColor /*= 0x00*/)
    {
        if (t_Height % 2 != 0 || t_Width % 2 != 0) {
            throw std::runtime_error("Image dimensions should be divisible by 2!");
        }

        m_ImageMatrix = std::move(ImageMatrix<Color8>(t_Height, t_Width, t_FillColor));
    }

    BmpImage::BmpImage(const std::string& t_ImagePath)
    {
        CImg<Color8> image(t_ImagePath.c_str());

        if (image.height() % 2 != 0 || image.width() % 2 != 0) {
            throw std::runtime_error("Image dimensions should be divisible by 2!");
        }

        m_ImageMatrix = std::move(ImageMatrix<Color8>(image.height(), image.width(), 0x0));

        cimg_forXY(image, imgX, imgY) {
            m_ImageMatrix.SetPixel(imgY, imgX, image(imgX, imgY));
        }
    }

    BmpImage::BmpImage(ImageMatrix<Color8>&& t_ImageMatrix)
    {
        m_ImageMatrix = std::move(t_ImageMatrix);
    }

    BmpImage::BmpImage(BmpImage&& t_Other)
    {
        m_ImageMatrix = std::move(t_Other.GetImageMatrix());
    }

    BmpImage& BmpImage::operator=(BmpImage&& t_Other) noexcept
    {
        if (this != &t_Other) {
            m_ImageMatrix = std::move(t_Other.GetImageMatrix());
        }

        return *this;
    }

    void BmpImage::Save(const std::string& t_ImagePath)
    {
        if (!t_ImagePath.ends_with(".bmp")) {
            throw std::invalid_argument("Image must be saved in BMP file format!");
        }

        CImg<Color8> image(m_ImageMatrix.GetWidth(), m_ImageMatrix.GetHeight(), 1, 1, 0);
        cimg_forXY(image, imgX, imgY) {
            image(imgX, imgY) = m_ImageMatrix.GetPixel(imgY, imgX);
        }

        image.save(t_ImagePath.c_str());
    }

    BmpImage BmpImage::Crop(uint32_t t_YStart, uint32_t t_YEnd, uint32_t t_XStart, uint32_t t_XEnd)
    {
        return std::move(BmpImage(std::move(m_ImageMatrix.Slice(t_YStart, t_YEnd, t_XStart, t_XEnd))));
    }

    BmpImage& BmpImage::SetPixel(uint32_t t_Y, uint32_t t_X, Color8 t_NewPixelValue)
    {
        m_ImageMatrix.SetPixel(t_Y, t_X, t_NewPixelValue);
        return *this;
    }

    Color8 BmpImage::GetPixel(uint32_t t_Y, uint32_t t_X) const
    {
        return m_ImageMatrix.GetPixel(t_Y, t_X);
    }

    uint32_t BmpImage::GetHeight() const
    {
        return m_ImageMatrix.GetHeight();
    }

    uint32_t BmpImage::GetWidth() const
    {
        return m_ImageMatrix.GetWidth();
    }

    ImageMatrix<Color8>& BmpImage::GetImageMatrix()
    {
        return m_ImageMatrix;
    }

    void BmpImage::Show()
    {
        CImg<Color8> image(m_ImageMatrix.GetWidth(), m_ImageMatrix.GetHeight(), 1, 1, 0);
        cimg_forXY(image, imgX, imgY) {
            image(imgX, imgY) = m_ImageMatrix.GetPixel(imgY, imgX);
        }

        image.display();
    }

    std::ostream& operator<<(std::ostream& t_Stream, BmpImage& t_BmpImage)
    {
        t_Stream << "{\n";
        for (auto rowIt = t_BmpImage.m_ImageMatrix.GetMatrixRaw().begin(); rowIt != t_BmpImage.m_ImageMatrix.GetMatrixRaw().end(); ++rowIt) {
            t_Stream << "    { ";
            for (auto elemIt = rowIt->begin(); elemIt != rowIt->end(); ++elemIt) {
                if (std::distance(elemIt, rowIt->end()) == 1) {
                    t_Stream << "0x" << std::hex << unsigned(*elemIt);
                }
                else {
                    t_Stream << "0x" << std::hex << unsigned(*elemIt) << ", ";
                }
            }
            if (std::distance(rowIt, t_BmpImage.m_ImageMatrix.GetMatrixRaw().end()) == 1) {
                t_Stream << " }\n";
            }
            else {
                t_Stream << " },\n";
            }
        }
        t_Stream << "}";

        return t_Stream;
    }
}

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

    BmpImage BmpImage::Slice(uint32_t t_XStart, uint32_t t_XEnd, uint32_t t_YStart, uint32_t t_YEnd)
    {
        assert(false && ("Function is not implemented!"));
        return BmpImage(0, 0);
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

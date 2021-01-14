#include "bmp_image.h"

#define cimg_display 0
#define cimg_use_png 0
#define cimg_use_jpeg 0
#include "CImg/CImg.h"
using namespace cimg_library;

namespace rdh {

    BmpImage::BmpImage(const std::string& t_ImagePath)
    {
        CImg<unsigned char> image(t_ImagePath.c_str());
    }

    BmpImage::~BmpImage()
    {

    }

}
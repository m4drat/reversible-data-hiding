#include <iostream>
#include <boost/program_options.hpp>
#include "image_matrix.h"
#include "bmp_image.h"

namespace po = boost::program_options;

int wmain(int argc, wchar_t* argv[])
{
    std::cout << "Everything is okay!" << std::endl;

    rdh::BmpImage im("..\\..\\..\\..\\images\\boat_small.bmp");
    //im.Show();
    std::cout << im;

    /**
    * All options:
    * --image-path - std::wstring
    * --mode
    * --encryption-key
    */
    //po::variables_map vm;
    //po::options_description desc{ "Options" };
    //desc.add_options()
    //    ("help,h", "Prints help message")
    //    ("image-path", po::value<std::wstring>()->required()->default_value(L"..\\..\\..\\..\\images\\boat.bmp"), "Path to an image");
    //    ("mode", po::value<std::wstring>()->required()->default_value(L"encrypt"), "Path to an image");

    return 0;
}

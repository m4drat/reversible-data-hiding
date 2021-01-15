#include <iostream>
#include <boost/program_options.hpp>
#include "image_matrix.h"
#include "bmp_image.h"

int main(int argc, char* argv[])
{
    std::cout << "Everything is okay!" << std::endl;

    rdh::BmpImage im("..\\..\\..\\..\\images\\boat.bmp");
    im.Show();
    im.Save("..\\..\\..\\..\\images\\boat-saved.bmp");

    return 0;
}

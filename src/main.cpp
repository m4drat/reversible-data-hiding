#include <iostream>
#include <boost/program_options.hpp>
#include "image_matrix.h"

int main(int argc, char* argv[])
{
    std::cout << "Everything is okay!" << std::endl;

    rdh::ImageMatrix<rdh::Color8> imMat;
    imMat.GetHeight();
    imMat.GetWidth();

    return 0;
}

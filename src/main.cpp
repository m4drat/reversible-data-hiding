#include <iostream>
#include <string>
#include <boost/program_options.hpp>

#include "image_matrix.h"
#include "bmp_image.h"

namespace po = boost::program_options;

int wmain(int argc, wchar_t* argv[])
{
    std::cout << "Everything is okay!" << std::endl;

    //rdh::BmpImage im("..\\..\\..\\..\\images\\boat_small.bmp");
    //im.Show();
    //std::cout << im;

    /**
    * All options:
    * --image-path - std::wstring
    * --result-path - std::wstring
    * --mode
    * --encryption-key
    * --data-key
    */
    po::variables_map vm;
    po::options_description desc{ "Options" };
    desc.add_options()
        ("help,h", "Prints help message.")
        ("image-path", po::wvalue<std::wstring>()->required()->default_value(L"..\\..\\..\\..\\images\\boat_small.bmp", "..\\..\\..\\..\\images\\boat_small.bmp"), "Path to an image to work with.")
        ("result-path", po::wvalue<std::wstring>()->required()->default_value(L"..\\..\\..\\..\\images\\result.bmp", "..\\..\\..\\..\\images\\result.bmp"), "Path to save decrypted/encrypted image or recovered message to.")
        ("mode", po::wvalue<std::wstring>()->required()->default_value(L"encrypt", "encrypt"), "Specifies execution mode.\n"
            "Can be one of the follows:\n"
            "  encrypt: \tEncrypts image specified in --image-path using key provided in --encryption-key. "
            "Result will be saved in --result-path.\n"
            "  hide   : \tHides data in image specified in --image-path using key provided in --data-key. "
            "Result will be saved in --result-path.\n"
            "  decrypt: \tDecrypts image specified in --image-path using key provided in --encryption-key. "
            "Result will be saved in --result-path.\n"
            "  extract: \tExtracts data from image specified in --image-path using key provided in --data-key. "
            "Result will be saved in --result-path.\n"
            "  recover: \tRecovers image specified in --image-path using both keys provided in --data-key and "
            "--encryption-key. Result will be saved in --result-path.\n")
        ("encryption-key", po::value<std::string>(), "Image encryption/decryption key.")
        ("data-key", po::value<std::string>(), "Data embedding/extraction key.");

    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.size() == 0 || vm.count("help"))
    {
        std::cout << desc << "\n";
        return 1;
    }

    return 0;
}

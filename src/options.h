#pragma once

#include <string>
#include <iostream>
#include <boost/program_options.hpp>

namespace rdh {
    /**
     * @brief Class that wraps handlers for all command line options.
    */
    namespace po = boost::program_options;

    class Options {
    public:
        /**
         * @brief Handles show command
         * @param t_ImagePath path to an image
         * @param t_Vm boost variables map
         * @param t_Desc boost options description
         * @return 0 if everything is OK, non-zero otherwise
        */
        static uint32_t HandleShow(const std::string& t_ImagePath, po::variables_map& t_Vm, po::options_description& t_Desc);

        /**
         * @brief Handles encrypt command
         * @param t_ImagePath path to an image
         * @param t_Vm boost variables map
         * @param t_Desc boost options description
         * @return 0 if everything is OK, non-zero otherwise
        */
        static uint32_t HandleEncrypt(const std::string& t_ImagePath, po::variables_map& t_Vm, po::options_description& t_Desc);
        
        /**
         * @brief Handles hide command
         * @param t_ImagePath path to an image
         * @param t_Vm boost variables map
         * @param t_Desc boost options description
         * @return 0 if everything is OK, non-zero otherwise
        */
        static uint32_t HandleEmbed(const std::string& t_ImagePath, po::variables_map& t_Vm, po::options_description& t_Desc);
        
        /**
         * @brief Handles decrypt command
         * @param t_ImagePath path to an image
         * @param t_Vm boost variables map
         * @param t_Desc boost options description
         * @return 0 if everything is OK, non-zero otherwise
        */
        static uint32_t HandleDecrypt(const std::string& t_ImagePath, po::variables_map& t_Vm, po::options_description& t_Desc);
        
        /**
         * @brief Handles extract command
         * @param t_ImagePath path to an image
         * @param t_Vm boost variables map
         * @param t_Desc boost options description
         * @return 0 if everything is OK, non-zero otherwise
        */
        static uint32_t HandleExtractAndRecover(const std::string& t_ImagePath, po::variables_map& t_Vm, po::options_description& t_Desc);

        /**
         * @brief Handles "calculate PSNR" command.
         * @param t_ImagePath1 path to the first image.
         * @param t_ImagePath2 path to the second image.
         * @param t_Vm boost variables map
         * @param t_Desc boost options description
         * @return 0 if everything is OK, non-zero otherwise
        */
        static uint32_t HandleCalculatePsnr(const std::string& t_ImagePath1, const std::string& t_ImagePath2, po::variables_map& t_Vm, po::options_description& t_Desc);

        /**
         * @brief Handles "calculate SSIM" command.
         * @param t_ImagePath1 path to the first image.
         * @param t_ImagePath2 path to the second image.
         * @param t_Vm boost variables map
         * @param t_Desc boost options description
         * @return 0 if everything is OK, non-zero otherwise
        */
        static uint32_t HandleCalculateSsim(const std::string& t_ImagePath1, const std::string& t_ImagePath2, po::variables_map& t_Vm, po::options_description& t_Desc);
    private:
        /**
         * @brief Extraction modes for extractor module.
        */
        enum class Mode {
            DATA_EXTRACT_IMAGE_RECOVERY,
            DATA_EXTRACT,
            IMAGE_RECOVERY,
            NONE
        };
    };
}
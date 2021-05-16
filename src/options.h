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
        static int HandleShow(const std::string& t_ImagePath, po::variables_map& t_Vm, po::options_description& t_Desc);

        /**
         * @brief Handles encrypt command
         * @param t_ImagePath path to an image
         * @param t_Vm boost variables map
         * @param t_Desc boost options description
         * @return 0 if everything is OK, non-zero otherwise
        */
        static int HandleEncrypt(const std::string& t_ImagePath, po::variables_map& t_Vm, po::options_description& t_Desc);
        
        /**
         * @brief Handles hide command
         * @param t_ImagePath path to an image
         * @param t_Vm boost variables map
         * @param t_Desc boost options description
         * @return 0 if everything is OK, non-zero otherwise
        */
        static int HandleEmbed(const std::string& t_ImagePath, po::variables_map& t_Vm, po::options_description& t_Desc);
        
        /**
         * @brief Handles decrypt command
         * @param t_ImagePath path to an image
         * @param t_Vm boost variables map
         * @param t_Desc boost options description
         * @return 0 if everything is OK, non-zero otherwise
        */
        static int HandleDecrypt(const std::string& t_ImagePath, po::variables_map& t_Vm, po::options_description& t_Desc);
        
        /**
         * @brief Handles extract command
         * @param t_ImagePath path to an image
         * @param t_Vm boost variables map
         * @param t_Desc boost options description
         * @return 0 if everything is OK, non-zero otherwise
        */
        static int HandleExtractAndRecover(const std::string& t_ImagePath, po::variables_map& t_Vm, po::options_description& t_Desc);
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
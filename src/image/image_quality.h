#pragma once 

#include "image/bmp_image.h"
#include "types.h"

namespace rdh {
    class ImageQuality {
    public:
        /**
         * @brief Calculates PSNR (Peak Signal To Noise Ratio) between two images.
         * @param t_Img1 first image.
         * @param t_Img2 second image.
         * @return PSNR value (can be +infinity). 
        */
        static double CalculatePSNR(const BmpImage& t_Img1, const BmpImage& t_Img2);

        /**
         * @brief Calculates SSIM (Structural Similarity) between two images.
         * @param t_Img1 first image.
         * @param t_Img2 second image.
         * @return SSIM value (can be +infinity). 
        */
        static double CalculateSSIM(const BmpImage& t_Img1, const BmpImage& t_Img2);
    private:
        struct Block {
            Block(Color8u t_Px1, Color8u t_Px2, Color8u t_Px3, Color8u t_Px4)
                : px1{ t_Px1 }, px2{ t_Px2 }, px3{ t_Px3 }, px4{ t_Px4 }
            {}
            Color8u px1;
            Color8u px2;
            Color8u px3;
            Color8u px4;

            /**
             * @brief Calculates mean for the current block.
             * @return double, the mean value.
            */
            double CalculateMean() const {
                return (double)(px1 + px2 + px3 + px4) / (double)4;
            }

            /**
             * @brief Calculates covariance standard deviation for the current block.
             * @return double, the standard deviation value.
            */
            double CalculateStandardDeviation() const {
                return std::sqrtf(
                    (
                        std::powf((px1 - CalculateMean()), 2) +
                        std::powf((px2 - CalculateMean()), 2) +
                        std::powf((px3 - CalculateMean()), 2) +
                        std::powf((px4 - CalculateMean()), 2)
                    ) / 4
                );
            }
            
            /**
             * @brief Calculates covariance between current and t_Other pixels blocks.
             * @param t_Other Second block, to calculate covariance with.
             * @return double, the covariance value.
            */
            double Covariance(const Block& t_Other) const {
                return ((
                    (px1 - CalculateMean()) * (t_Other.px1 - t_Other.CalculateMean()) + 
                    (px2 - CalculateMean()) * (t_Other.px2 - t_Other.CalculateMean()) +
                    (px3 - CalculateMean()) * (t_Other.px3 - t_Other.CalculateMean()) +
                    (px4 - CalculateMean()) * (t_Other.px4 - t_Other.CalculateMean()))
                ) / 3;
            }
        };

        static double Phi(const Block& block1, const Block& block2);

        static constexpr double s_Epsilon1{ 1E-10 + std::numeric_limits<double>::epsilon() };
        static constexpr double s_Epsilon2{ 1E-10 - std::numeric_limits<double>::epsilon() };
    };
}
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
         * The value of SSIM index belongs to [0, 1].
         * @param t_Img1 first image.
         * @param t_Img2 second image.
         * @return SSIM value (can be +infinity). 
         */
        static double CalculateSSIM(const BmpImage& t_Img1, const BmpImage& t_Img2);
    
        /**
         * @brief Represents 2x2 pixels block.
         */
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
                return ((double)px1 + (double)px2 + (double)px3 + (double)px4) / 4.0f;
            }

            /**
             * @brief Calculates covariance standard deviation for the current block.
             * @return double, the standard deviation value.
             */
            double CalculateStandardDeviation() const {
                return std::sqrtf(
                    (
                        std::powf(((double)px1 - CalculateMean()), 2) +
                        std::powf(((double)px2 - CalculateMean()), 2) +
                        std::powf(((double)px3 - CalculateMean()), 2) +
                        std::powf(((double)px4 - CalculateMean()), 2)
                    ) / 3.0f
                );
            }

            /**
             * @brief Calculates covariance between current and t_Other pixels blocks.
             * @param t_Other Second block, to calculate covariance with.
             * @return double, the covariance value.
             */
            double Covariance(const Block& t_Other) const {
                return ((
                    ((double)px1 - CalculateMean()) * ((double)t_Other.px1 - t_Other.CalculateMean()) +
                    ((double)px2 - CalculateMean()) * ((double)t_Other.px2 - t_Other.CalculateMean()) +
                    ((double)px3 - CalculateMean()) * ((double)t_Other.px3 - t_Other.CalculateMean()) +
                    ((double)px4 - CalculateMean()) * ((double)t_Other.px4 - t_Other.CalculateMean()))
                ) / 3.0f;
            }
        };
    private:
        static double Phi(const Block& block1, const Block& block2);

        static constexpr double s_Const1{ 6.5025f };
        static constexpr double s_Const2{ 58.5225f };
    };
}
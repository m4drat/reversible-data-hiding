#include "image/image_quality.h"

#include <cmath>
#include <limits>

namespace rdh {
    double ImageQuality::CalculatePSNR(const BmpImage& t_Img1, const BmpImage& t_Img2)
    {
        if ((t_Img1.GetHeight() != t_Img2.GetHeight()) || (t_Img1.GetWidth() != t_Img2.GetWidth())) {
            throw std::invalid_argument("The dimensions of the images must be the same!");
        }

        double meanSquaredError = 0;

        for (uint32_t imgY = 0; imgY < t_Img1.GetHeight(); imgY += 1) {
            for (uint32_t imgX = 0; imgX < t_Img1.GetWidth(); imgX += 1) {
                meanSquaredError += std::powl(t_Img1.GetPixel(imgY, imgX) - t_Img2.GetPixel(imgY, imgX), 2);
            }
        }

        meanSquaredError /= ((double)t_Img1.GetHeight() * (double)t_Img1.GetWidth());

        /* If MSE is almost 0, return +infinity */
        if (meanSquaredError <= s_Epsilon1) {
            return +std::numeric_limits<double>::infinity();
        }

        return 10 * std::log10f(std::powf(255, 2) / meanSquaredError);
    }

    double ImageQuality::CalculateSSIM(const BmpImage& t_Img1, const BmpImage& t_Img2)
    {
        if ((t_Img1.GetHeight() != t_Img2.GetHeight()) || (t_Img1.GetWidth() != t_Img2.GetWidth())) {
            throw std::invalid_argument("The dimensions of the images must be the same!");
        }

        double ssim = 0;

        for (uint32_t imgY = 0; imgY < t_Img1.GetHeight(); imgY += 2) {
            for (uint32_t imgX = 0; imgX < t_Img1.GetWidth(); imgX += 2) {
                ssim += Phi(
                    Block(
                        t_Img1.GetPixel(imgY, imgX),
                        t_Img1.GetPixel(imgY, imgX + 1),
                        t_Img1.GetPixel(imgY + 1, imgX),
                        t_Img1.GetPixel(imgY + 1, imgX + 1)
                    ),
                    Block(
                        t_Img2.GetPixel(imgY, imgX),
                        t_Img2.GetPixel(imgY, imgX + 1),
                        t_Img2.GetPixel(imgY + 1, imgX),
                        t_Img2.GetPixel(imgY + 1, imgX + 1)
                    )
                );
            }
        }

        ssim /= ((double)t_Img1.GetHeight() * (double)t_Img1.GetWidth() / 4.0f);

        return ssim;
    }

    double ImageQuality::Phi(const Block& block1, const Block& block2)
    {
        double numerator = (2 * block1.CalculateMean() * block2.CalculateMean() + s_Epsilon1) * (2 * block1.Covariance(block2) + s_Epsilon2);
        double denominator = (std::powf(block1.CalculateMean(), 2) + std::powf(block2.CalculateMean(), 2) + s_Epsilon1) *
            (std::powf(block1.CalculateStandardDeviation(), 2) + std::powf(block2.CalculateStandardDeviation(), 2) + s_Epsilon2);

        return numerator / denominator;
    }
}
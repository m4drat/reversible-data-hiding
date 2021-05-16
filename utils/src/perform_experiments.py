# -*- coding: utf-8 -*-
# @Author: madrat

import math
import cv2
import numpy as np

from skimage.metrics import structural_similarity

def calculate_psnr(img1, img2):
    mse = np.square(np.subtract(img1, img2)).mean()

    if mse == 0:
        return 100

    return 10 * math.log10(255.0 ** 2 / mse)

def main():
    # img1 = cv2.imread('../../images/lena_gray-embed.bmp')
    # img2 = cv2.imread('../../images/lena_gray-embed.bmp')

    img1 = cv2.imread('../../../../../man-1.png')
    img2 = cv2.imread('../../../../../man-2.png')

    print(f'PSNR: {calculate_psnr(img1, img2)} db')
    print(f'SSIM: {structural_similarity(img1, img2, multichannel=True)} db')

if __name__ == "__main__":
    main()
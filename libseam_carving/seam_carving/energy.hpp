//
// Created by Andy Mina on 3/7/23.
//
#ifndef SEAM_CARVING_ENERGY_HPP
#define SEAM_CARVING_ENERGY_HPP

// 3rd party
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

// std
#include <algorithm>
#include <limits>

namespace seam_carving::energy {
    /**
     * @brief Computes the energy of an image using the Scharr kernel.
     *
     * Computes the energy of an image using the gradient and returns a grayscale
     * images where the brightness of a pixel is propotional to its energy. Uses the
     * energy function defined as described in Avidan et al., E(p) = | dx(p) | + |
     * dy(p) |, where p represents a pixel and dx/dy represent the first
     * derivatives. In the paper the first derivatives are found using the Sobel
     * kernel, but this function uses the Scharr kernel for better accuracy.
     *
     * @param img an image read by OpenCV
     * @returns an image representing the energy of the original image
     *
     * TODO(#22): generate a proper doxygen comment
    */
    void ComputeEnergy(cv::InputArray in_img, cv::OutputArray out_img);

    /**
     * @brief Computes the energy map, used to find the path of least energy, of an
     * image.
     *
     * The energy map is visual representation of the paths of least energy where
     * the brightness is proportional to the energy of the pixel. Seams can be
     * created by following the path of least energy. The vertical path of least
     * energy is found by the following algorithm:
     *    1. initalize the bottom row with weights equal to their energy
     *    2. move to the next row above (row - 1 -> row)
     *    3. loop through each pixel in the next row
     *    4. compute the weights adjacent pixels in the row underneath, specified
     * below. if an adjacent pixel is OOB, set it to max. center  (row + 1, col)
     *          left    (row + 1, col - 1)
     *          right   (row + 1, col +1)
     *    5. set the current pixel's weight equal to its energy plus the min weight
     *       of the adjacent pixels calculated in step 4
     *    6. repeat steps 2-5 until the weights of all pixels are calculated
     *
     * @param energy_img a gradient image
     * @returns the energy map of energy_img
    */
    void ComputeVerticalEnergyMap(cv::InputArray input, cv::OutputArray output);
    void ComputeHorizontalEnergyMap(cv::InputArray input, cv::OutputArray output);
}

#endif //SEAM_CARVING_ENERGY_HPP

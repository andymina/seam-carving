/**
 * Andy Mina
 * 
 * Definition of all "energy" functions used in seam carving.
*/
#ifndef energy_h
#define energy_h

#include <algorithm>
#include <limits>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "constants.h"

namespace SeamCarving {
  namespace Energy {
    /**
     * Computes the energy of an image using the gradient and returns a grayscale images where the
     * brightness of a pixel is propotional to its energy. Uses the energy function defined as
     * described in Avidan et al., E(p) = | dx(p) | + | dy(p) |, where p represents a pixel and
     * dx/dy represent the first derivatives.
     * 
     * @param img an image read by OpenCV
     * @param kernel the kernel to use to compute the gradient. Must be one of ["sobel", "scharr"].
     *  Defaults to "sobel"
     * @param dir the direction to compute the energy in. Must be one of [VERT, HORZ]. Defaults to VERT.
     * 
     * @returns an image representing the energy of the original image
    */
    Image ComputeEnergy(const Image &img, const std::string &kernel = "sobel");
    

    /**
     * Computes the energy map and the path map for a given energy image.
     * 
     * The energy map is visual representation of the paths of least energy where the brightness is
     * proportional to the energy of the pixel. Seams can be created by following the path of least
     * energy. The vertical path of least energy is found by the following algorithm:
     *    1. initalize the bottom row with weights equal to their energy
     *    2. move to the next row above (row - 1 -> row)
     *    3. loop through each pixel in the next row
     *    4. compute the weights adjacent pixels in the row underneath, specified below. if an
     *       adjacent pixel is OOB, set it to max.
     *          center  (row + 1, col)
     *          left    (row + 1, col - 1)
     *          right   (row + 1, col +1)
     *    5. set the current pixel's weight equal to its energy plus the min weight
     *       of the adjacent pixels calculated in step 4
     *    6. repeat steps 2-5 until the weights of all pixels are calculated
     * 
     * @param energy_img an Image representing the gradient of the image
     * @returns the energy map of the Image
    */
    Image ComputeVerticalEnergyMap(const Image &energy_img);
    Image ComputeHorizontalEnergyMap(const Image &energy_img);
  }
}

#endif /** energy_h */
/**
 * Andy Mina
 * 
 * Definition of all "energy" functions used in seam carving.
*/
#ifndef energy_h
#define energy_h

#include <algorithm>
#include <iostream>
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
     * Computes the vertical energy map and the path map for a given energy image.
     * 
     * The energy map is visual representation of the paths of least energy where the brightness is
     * proportional to the energy of the pixel. Seams can be created by following the path of least
     * energy.
     * 
     * @param energy_img an Image representing the gradient of the image
     * 
     * @returns the energy map of the Image
    */
    Image ComputeEnergyMap(const Image &energy_img);
  }
}

#endif /** energy_h */
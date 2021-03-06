/**
 * Andy Mina
 * 
 * Implementation of the functions defined in energy.h
*/
#include "energy.h"

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
    Image ComputeEnergy(const Image &img, const std::string &kernel) {
      // apply gaussian and convert grayscale
      Image src;
      cv::cvtColor(img, src, cv::COLOR_BGR2GRAY);
      cv::GaussianBlur(src, src, cv::Size(3, 3), 0, 0);
      

      // apply kernel
      Image x_edges, y_edges;
      int ksize = (kernel == "scharr") ? -1 : 3;
      cv::Sobel(src, x_edges, CV_16S, 1, 0, ksize);
      cv::Sobel(src, y_edges, CV_16S, 0, 1, ksize);

      // convert and merge
      Image abs_x, abs_y;
      cv::convertScaleAbs(x_edges, abs_x);
      cv::convertScaleAbs(y_edges, abs_y);
      cv::addWeighted(abs_x, 0.5, abs_y, 0.5, 0, src);
      return src;
    }

    /**
     * Computes the vertical energy map and the path map for a given energy image.
     * 
     * The energy map is visual representation of the paths of least energy where the brightness is
     * proportional to the energy of the pixel. Seams can be created by following the path of least
     * energy.
     * 
     * @param energy_img an Image representing the gradient of the image
     * @returns the energy map of the Image
    */
    Image ComputeEnergyMap(const Image &energy_img) {
      // setup
      int rows = energy_img.rows;
      int cols = energy_img.cols;
      Image energy_map = Image(rows, cols, CV_32S); //, cv::Scalar(0.0));
      energy_img.row(rows - 1).copyTo(energy_map.row(rows - 1));

      for (int row = rows - 2; row >= 0; row--) {
        for (int col = 0; col < cols; col++) {
          // compute vals
          const int &center = energy_map.at<int>(row + 1, col);
          const int &left = (col - 1 < 0) ? std::numeric_limits<int>::max() : energy_map.at<int>(row + 1, col - 1);
          const int &right = (col + 1 >= cols) ? std::numeric_limits<int>::max() : energy_map.at<int>(row + 1, col + 1);

          // take the min energy plus self
          energy_map.at<int>(row, col) = energy_img.at<uchar>(row, col) + std::min({center, left, right});
        }
      }

      return energy_map;
    }
  }
}
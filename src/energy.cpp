/**
 * Andy Mina
 * 
 * Implementation of the functions defined in energy.h
*/
#include "energy.h"

namespace SeamCarving {
  namespace Energy {
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

    Image ComputeVerticalEnergyMap(const Image &energy_img) {
      // setup
      int rows = energy_img.rows;
      int cols = energy_img.cols;
      Image energy_map = Image(rows, cols, CV_32S);

      // step 1: initialize the bottom row
      energy_img.row(rows - 1).copyTo(energy_map.row(rows - 1));

      // step 2: move to the next row
      for (int row = rows - 2; row >= 0; row--) {
        // step 3: loop through all pixels in this row (cols)
        for (int col = 0; col < cols; col++) {
          // step 4: compute weights of adjacent pixels
          const int &center = energy_map.at<int>(row + 1, col);
          const int &left = (col - 1 < 0) ? std::numeric_limits<int>::max() : energy_map.at<int>(row + 1, col - 1);
          const int &right = (col + 1 >= cols) ? std::numeric_limits<int>::max() : energy_map.at<int>(row + 1, col + 1);

          // step 5: update the current pixel's weight
          energy_map.at<int>(row, col) = energy_img.at<uchar>(row, col) + std::min({center, left, right});
        }
      }

      return energy_map;
    }

    Image ComputeHorizontalEnergyMap(const Image &energy_img) {
      // setup
      int rows = energy_img.rows;
      int cols = energy_img.cols;
      Image energy_map = Image(rows, cols, CV_32S);

      // step 1: initial the right col
      energy_img.col(cols - 1).copyTo(energy_map.col(cols - 1));

      // step 2: move to the next col
      for (int col = cols - 2; col >= 0; col--) {
        // step 3: loop through all pixels in this col
        for (int row = 0; row < rows; row++) {
          // step 4: compute weights of adjacent pixels
          const int &center = energy_map.at<int>(row, col + 1);
          const int &left = (row - 1 < 0) ? std::numeric_limits<int>::max() : energy_map.at<int>(col + 1, row - 1);
          const int &right = (row + 1 >= rows) ? std::numeric_limits<int>::max() : energy_map.at<int>(col + 1, row + 1);

          // step 5: update the current pixel's weight
          energy_map.at<int>(row, col) = energy_img.at<uchar>(row, col) + std::min({center, left, right});
        }
      }

      return energy_map;
    }
  }
}
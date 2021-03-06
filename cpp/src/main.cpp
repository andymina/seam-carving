/**
 * Andy Mina
 * 
 * File to test SeamCarver in C++ implementation. Given a path to an image, the target number of
 * rows, and the target number of cols, carves the image and displays the result.
 * 
 * Args:
 *    {path to img}
 *    {target rows}
 *    {target cols}
*/
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "CarvableImage.h"

namespace sc = SeamCarving;

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cout << "Usage: " << argv[0] << " {path to img} {target rows} {target cols}\n";
    return -1;
  }

  const string input_path(argv[1]);
  const int target_rows(stoi(argv[2]));
  const int target_cols(stoi(argv[3]));

  sc::CarvableImage img = sc::CarvableImage(input_path);
  std::cout << "Original size: " << img.num_rows() << ", " << img.num_cols() << "\n";

  img.SeamCarve(target_rows, target_cols);
  std::cout << "Carved size: " << img.num_rows() << ", " << img.num_cols() << "\n";
  
  cv::imshow("Original", img.GetOriginal());
  cv::imshow("Carved", img.GetResult());
  int k = cv::waitKey(0);
  return 0;
}
#include <iostream>
#include <vector>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "CarvableImage.h"

using std::cout;
using std::ofstream;
namespace sc = SeamCarving;
namespace nrg = sc::Energy;

int main() {
  sc::CarvableImage img = sc::CarvableImage("../samples/dali.jpeg");
  std::cout << "OG size: " << img.num_rows() << ", " << img.num_cols() << "\n";

  img.SeamCarve(590, 770); // 570 x 750
  std::cout << "new size: " << img.num_rows() << ", " << img.num_cols() << "\n";
  
  cv::imshow("og", img.GetOriginal());
  cv::imshow("cpp", img.GetResult());
  int k = cv::waitKey(0);
  return 0;
}
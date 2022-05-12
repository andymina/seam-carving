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
  std::vector<sc::Seam> seams = img.FindKOptimalSeams(100);
  img.HighlightKSeams(seams, 176, 38, 255);
  cv::imshow("cpp", img.GetResult());
  int k = cv::waitKey(0);
  return 0;
}
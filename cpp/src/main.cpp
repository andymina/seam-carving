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

  std::vector<sc::Seam> seams = img.FindKOptimalSeams(100, sc::Dir::HORZ);
  
  img.HighlightKSeams(seams);
  img.Export(sc::ImageType::RESULT, "out.jpg");

  img.Reset();
  img.InsertKSeams(100, sc::Dir::HORZ);
  std::cout << "new size: " << img.num_rows() << ", " << img.num_cols() << "\n";

  cv::imshow("cpp", img.GetResult());
  int k = cv::waitKey(0);
  return 0;
}
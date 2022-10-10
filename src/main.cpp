/**
 * Andy Mina
 *
 * File to test SeamCarver in C++ implementation. Given a path to an image, the
 * target number of rows, and the target number of cols, carves the image and
 * displays the result.
 *
 * Args:
 *    {path to img}
 *    {target rows}
 *    {target cols}
 */
// sys
#include <iostream>
#include <string>

// external
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

/** TODO(#15): uncomment this when done */
// // project
// #include "CarvableImage.h"

// namespace sc = SeamCarving;

// int main(int argc, char **argv) {
//     if (argc != 4) {
//         std::cout << "Usage: " << argv[0] << " {path to img} {target rows}
//         {target cols}\n"; return -1;
//     }

//     const std::string input_path(argv[1]);
//     const int target_rows(std::stoi(argv[2]));
//     const int target_cols(std::stoi(argv[3]));

//     sc::CarvableImage img = sc::CarvableImage(input_path);
//     std::cout << "Original size: " << img.num_rows() << ", " <<
//     img.num_cols() << "\n";

//     img.SeamCarve(target_rows, target_cols);
//     std::cout << "Carved size: " << img.num_rows() << ", " << img.num_cols()
//     << "\n";

//     cv::imshow("Original", img.GetOriginal());
//     cv::imshow("Carved", img.GetResult());
//     int k = cv::waitKey(0);
//     return 0;
// }
#include "energy.h"

namespace nrg = SeamCarving::Energy;

int main() {
	cv::Mat img = cv::imread("samples/dali.jpeg");
	cv::Mat original = img.clone();
	cv::imshow("original", original);

	nrg::ComputeEnergy(original);

	nrg::ComputeVerticalEnergyMap(original, img);
	cv::normalize(img, img, 0, 255, cv::NORM_MINMAX);
	img.convertTo(img, CV_8U);
	cv::imshow("vertical", img);

	nrg::ComputeHorizontalEnergyMap(original, img);
	cv::normalize(img, img, 0, 255, cv::NORM_MINMAX);
	img.convertTo(img, CV_8U);
	cv::imshow("horiztonal", img);

	std::cout << "depth: " << img.depth() << "\n";

	int k = cv::waitKey(0);
}
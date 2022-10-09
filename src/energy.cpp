/**
 * Andy Mina
 *
 * Implementation of the functions defined in energy.h
 */
#include "energy.h"

namespace SeamCarving {
namespace Energy {
void ComputeEnergy(cv::InputOutputArray img) {
	// apply gaussian and convert grayscale
	cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
	cv::GaussianBlur(img, img, cv::Size(3, 3), 0, 0);

	/**
	 * apply kernel. set the depth on the kernel results to be 16-bit signed
	 * ints to avoid overflow since img.depth() will be CV_8U;
	 */
	cv::Mat x_nrg, y_nrg;
	cv::Scharr(img, x_nrg, CV_16S, 1, 0);
	cv::Scharr(img, y_nrg, CV_16S, 0, 1);

	// convert back to CV_16S depth and merge
	cv::convertScaleAbs(x_nrg, x_nrg);
	cv::convertScaleAbs(y_nrg, y_nrg);
	cv::addWeighted(x_nrg, 0.5, y_nrg, 0.5, 0, img);
}

cv::Mat ComputeVerticalEnergyMap(const cv::Mat &energy_img) {
	// setup
	int rows = energy_img.rows;
	int cols = energy_img.cols;
	cv::Mat energy_map = cv::Mat(rows, cols, CV_32S);

	// step 1: initialize the bottom row
	energy_img.row(rows - 1).copyTo(energy_map.row(rows - 1));

	// step 2: move to the next row
	for (int row = rows - 2; row >= 0; row--) {
		// step 3: loop through all pixels in this row (cols)
		for (int col = 0; col < cols; col++) {
			// step 4: compute weights of adjacent pixels
			const int &center = energy_map.at<int>(row + 1, col);
			const int &left = (col - 1 < 0)
								  ? std::numeric_limits<int>::max()
								  : energy_map.at<int>(row + 1, col - 1);
			const int &right = (col + 1 >= cols)
								   ? std::numeric_limits<int>::max()
								   : energy_map.at<int>(row + 1, col + 1);

			// step 5: update the current pixel's weight
			energy_map.at<int>(row, col) = energy_img.at<uchar>(row, col) +
										   std::min({center, left, right});
		}
	}

	return energy_map;
}

cv::Mat ComputeHorizontalEnergyMap(const cv::Mat &energy_img) {
	// setup
	int rows = energy_img.rows;
	int cols = energy_img.cols;
	cv::Mat energy_map = cv::Mat(rows, cols, CV_32S);

	// step 1: initial the right col
	energy_img.col(cols - 1).copyTo(energy_map.col(cols - 1));

	// step 2: move to the next col
	for (int col = cols - 2; col >= 0; col--) {
		// step 3: loop through all pixels in this col
		for (int row = 0; row < rows; row++) {
			// step 4: compute weights of adjacent pixels
			const int &center = energy_map.at<int>(row, col + 1);
			const int &left = (row - 1 < 0)
								  ? std::numeric_limits<int>::max()
								  : energy_map.at<int>(col + 1, row - 1);
			const int &right = (row + 1 >= rows)
								   ? std::numeric_limits<int>::max()
								   : energy_map.at<int>(col + 1, row + 1);

			// step 5: update the current pixel's weight
			energy_map.at<int>(row, col) = energy_img.at<uchar>(row, col) +
										   std::min({center, left, right});
		}
	}

	return energy_map;
}
} // namespace Energy
} // namespace SeamCarving
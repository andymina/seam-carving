/**
 * Andy Mina
 *
 * Implementation of the functions defined in energy.h
 */
#include "energy.h"

namespace SeamCarving {
namespace Energy {
    void ComputeEnergy(cv::InputArray in_img, cv::OutputArray out_img) {
        // apply gaussian and convert grayscale
        cv::GaussianBlur(in_img, out_img, cv::Size(3, 3), 0, 0);
        cv::cvtColor(out_img, out_img, cv::COLOR_BGR2GRAY);

        /**
         * apply kernel. set the depth on the kernel results to be 16-bit
         * signed ints to avoid overflow since img.depth() will be CV_8U;
         */
        cv::Mat x_nrg, y_nrg;
        /** TODO(#23): investigate if Scharr is better */
        cv::Sobel(out_img, x_nrg, CV_16S, 1, 0);
        cv::Sobel(out_img, y_nrg, CV_16S, 0, 1);

        // convert back to CV_8U depth and merge
        cv::convertScaleAbs(x_nrg, x_nrg);
        cv::convertScaleAbs(y_nrg, y_nrg);
        cv::addWeighted(x_nrg, 0.5, y_nrg, 0.5, 0, out_img);
    }

    void ComputeVerticalEnergyMap(cv::InputArray input, cv::OutputArray output) {
        // setup
        cv::Mat input_mat = input.getMat();
        int rows = input_mat.rows, cols = input_mat.cols;
        cv::Mat res = cv::Mat(rows, cols, CV_16U);

        // step 1: initialize the bottom row
        input_mat.row(rows - 1).copyTo(res.row(rows - 1));

        // step 2: move to the next row
        for (int row = rows - 2; row >= 0; row--) {
            // step 3: loop through all pixels in this row (cols)
            for (int col = 0; col < cols; col++) {
                // step 4: compute weights of adjacent pixels
                const ushort &center = res.at<ushort>(row + 1, col);
                const ushort &left = (col - 1 < 0) ? std::numeric_limits<ushort>::max() : res.at<ushort>(row + 1, col - 1);
                const ushort &right = (col + 1 >= cols) ? std::numeric_limits<ushort>::max() : res.at<ushort>(row + 1, col + 1);

                // step 5: update the current pixel's weight
                res.at<ushort>(row, col) = input_mat.at<uchar>(row, col) + std::min({center, left, right});
            }
        }

        res.copyTo(output); // output depth is CV_16U
    }

    void ComputeHorizontalEnergyMap(cv::InputArray input, cv::OutputArray output) {
        // setup
        cv::Mat input_mat = input.getMat();
        int rows = input_mat.rows, cols = input_mat.cols;
        cv::Mat res = cv::Mat(rows, cols, CV_16U);

        // step 1: initialize the right col
        input_mat.col(cols - 1).copyTo(res.col(cols - 1));

        // step 2: move to the next col
        for (int col = cols - 2; col >= 0; col--) {
            // step 3: loop through all pixels in this col
            for (int row = 0; row < rows; row++) {
                // step 4: compute weights of adjacent pixels
                const ushort &center = res.at<ushort>(row, col + 1);
                const ushort &above = (row - 1 < 0) ? std::numeric_limits<ushort>::max() : res.at<ushort>(row - 1, col + 1);
                const ushort &below = (row + 1 >= rows) ? std::numeric_limits<ushort>::max() : res.at<ushort>(row + 1, col + 1);

                // step 5: update the current pixel's weight
                res.at<ushort>(row, col) = input_mat.at<uchar>(row, col) +
                                           std::min({center, below, above});
            }
        }

        res.copyTo(output); // output depth is CV_16U
    }
} // namespace Energy
} // namespace SeamCarving
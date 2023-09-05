//
// Created by Andy Mina on 3/7/23.
//

#include <iostream>
#include <seam_carving/energy.hpp>

namespace seam_carving::energy {
    void ComputeEnergy(cv::InputArray in_img, cv::OutputArray out_img) {
        cv::GaussianBlur(in_img, out_img, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);

        // conversion should be skipped if img is grayscale already
        if (in_img.channels() != 1)
            cv::cvtColor(out_img, out_img, cv::COLOR_BGR2GRAY, 0);

        /**
         * apply kernel. set the depth on the kernel results to be 16-bit
         * signed ints to avoid overflow since img.depth() will be CV_8U;
         */
        cv::Mat x_nrg, y_nrg;
        /** TODO(#23): investigate if Scharr is better */
        if (in_img.channels() != 1) {
            cv::Sobel(out_img, x_nrg, CV_16S, 1, 0);
            cv::Sobel(out_img, y_nrg, CV_16S, 0, 1);
        } else {
            cv::Sobel(in_img, x_nrg, CV_16S, 1, 0);
            cv::Sobel(in_img, y_nrg, CV_16S, 0, 1);
        }

        std::cout << "post-sobel!\n";
        std::cout << "x\n" << x_nrg << "\n";
        std::cout << "y\n" << y_nrg << "\n\n";

        // convert back to CV_8U depth and merge
        cv::convertScaleAbs(x_nrg, x_nrg);
        cv::convertScaleAbs(y_nrg, y_nrg);

        std::cout << "post-abs!\n";
        std::cout << "x\n" << x_nrg << "\n";
        std::cout << "y\n" << y_nrg << "\n\n";
        cv::addWeighted(x_nrg, 0.5, y_nrg, 0.5, 0, out_img);

        std::cout << out_img.getMat() << "out\n";
    }

    void ComputeVerticalMap(cv::InputArray sobel, cv::OutputArray output) {
        // setup
        cv::Mat input_mat = sobel.getMat();
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

    void ComputeHorizontalMap(cv::InputArray sobel, cv::OutputArray output) {
        // setup
        cv::Mat input_mat = sobel.getMat();
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
}

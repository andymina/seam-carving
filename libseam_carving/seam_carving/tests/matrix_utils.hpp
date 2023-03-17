//
// Created by Andy Mina on 3/13/23.
//

#ifndef SEAM_CARVING_MATRIX_UTILS_HPP
#define SEAM_CARVING_MATRIX_UTILS_HPP

// 3rd party
#include <opencv2/core.hpp>

namespace seam_carving::tests {
    inline bool equalMatrices(const cv::Mat& lhs, const cv::Mat& rhs) {
        return cv::countNonZero(lhs != rhs) == 0;
    }
}

#endif //SEAM_CARVING_MATRIX_UTILS_HPP

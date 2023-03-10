//
// Created by Andy Mina on 3/9/23.
//

#include <seam_carving/tests/matrix_data.hpp>

namespace seam_carving::tests {
    bool equalMatrices(const cv::Mat& lhs, const cv::Mat& rhs) {
        return cv::countNonZero(lhs != rhs) == 0;
    }

    std::string PrintToString(const cv::Mat& matrix, const std::string& label) {
        std::stringstream ss;
        ss << ">>> " << label << "\n" << matrix;
        return ss.str();
    }
}

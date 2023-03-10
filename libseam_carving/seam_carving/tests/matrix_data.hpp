//
// Created by Andy Mina on 3/7/23.
//

#ifndef SEAM_CARVING_MATRIX_DATA_HPP
#define SEAM_CARVING_MATRIX_DATA_HPP

// 3rd party
#include <nlohmann/json.hpp>
#include <opencv2/core.hpp>

// std
#include <vector>

NLOHMANN_JSON_NAMESPACE_BEGIN
template<>
struct adl_serializer<cv::Mat> {
    static void from_json(const nlohmann::json& j, cv::Mat& matrix) {
        int rows = j.at("rows"), cols = j.at("cols");
        int depth = j.at("depth");
        std::vector<int> data = j.at("data");
        matrix = cv::Mat(rows, cols, CV_32S, data.data()).clone();
        matrix.convertTo(matrix, depth);
    }
};
NLOHMANN_JSON_NAMESPACE_END

namespace seam_carving::tests {
    bool equalMatrices(const cv::Mat& lhs, const cv::Mat& rhs);

    std::string PrintToString(const cv::Mat& matrix, const std::string& label = "");
}

#endif //SEAM_CARVING_MATRIX_DATA_HPP

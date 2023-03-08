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
        std::vector<int> data = j.at("data");
        matrix = cv::Mat(rows, cols, CV_32S, data.data()).reshape(1, rows);
    }
};
NLOHMANN_JSON_NAMESPACE_END

#endif //SEAM_CARVING_MATRIX_DATA_HPP

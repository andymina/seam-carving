//
// Created by Andy Mina on 3/11/23.
//

#ifndef SEAM_CARVING_JSON_UTILS_HPP
#define SEAM_CARVING_JSON_UTILS_HPP

// project
#include <seam_carving/coord.hpp>
#include <seam_carving/seam.hpp>
#include <seam_carving/tests/test_data.hpp>
#include <seam_carving/tests/energy_data.hpp>
#include <seam_carving/tests/carver_data.hpp>

// 3rd party
#include <opencv2/core.hpp>
#include <nlohmann/json.hpp>

// stl
#include <string>
#include <sstream>

namespace seam_carving {
    void to_json(nlohmann::json& j, const Coord& coord);
    void from_json(const nlohmann::json& j, Coord& coord);

    void to_json(nlohmann::json& j, const Seam& seam);
    void from_json(const nlohmann::json& j, Seam& seam);
}

namespace seam_carving::tests {
    void from_json(const nlohmann::json& j, TestData& test_data);
    void from_json(const nlohmann::json& j, EnergyData& energy_data);
    void from_json(const nlohmann::json& j, CarverData& carver_data);
}

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

#endif //SEAM_CARVING_JSON_UTILS_HPP

//
// Created by Andy Mina on 3/7/23.
//

#ifndef SEAM_CARVING_CARVER_DATA_HPP
#define SEAM_CARVING_CARVER_DATA_HPP

// project
#include <seam_carving/seam.hpp>
#include <seam_carving/tests/test_data.hpp>

// 3rd party
#include <nlohmann/json.hpp>
#include <opencv2/core.hpp>

namespace seam_carving {
    struct CarverTestData : TestData {
        Seam vertical_seam, horizontal_seam;
        cv::Mat remove_vertical_matrix, remove_horizontal_matrix;
        cv::Mat insert_vertical_matrix, insert_horizontal_matrix;
    };

    inline void from_json(const nlohmann::json& j, CarverTestData& carver_data) {
        const nlohmann::json& root = j.at("carver");

        carver_data.vertical_seam = root.at("find - vertical");
        carver_data.horizontal_seam =  root.at("find - horizontal");

        carver_data.remove_vertical_matrix = root.at("remove - vertical");
        carver_data.remove_horizontal_matrix = root.at("remove - horizontal");
        carver_data.insert_vertical_matrix = root.at("insert - vertical");
        carver_data.insert_horizontal_matrix = root.at("insert - horizontal");
    }
}

#endif //SEAM_CARVING_CARVER_DATA_HPP

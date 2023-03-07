//
// Created by Andy Mina on 3/7/23.
//

#ifndef SEAM_CARVING_ENERGY_DATA_HPP
#define SEAM_CARVING_ENERGY_DATA_HPP

// project
#include <seam_carving/tests/test_data.hpp>

// 3rd party
#include <nlohmann/json.hpp>
#include <opencv2/core.hpp>

namespace seam_carving {
    struct EnergyData : TestData {
        cv::Mat sobel_matrix;
        cv::Mat vertical_map_matrix, horizontal_map_matrix;
    };

    inline void from_json(const nlohmann::json& j, EnergyData& energy_data) {
        const nlohmann::json& root = j.at("energy");

        energy_data.sobel_matrix = root.at("sobel");

        energy_data.vertical_map_matrix = root.at("map - vertical");
        energy_data.horizontal_map_matrix = root.at("map - horizontal");
    }
}

#endif //SEAM_CARVING_ENERGY_DATA_HPP

//
// Created by Andy Mina on 3/7/23.
//

#ifndef SEAM_CARVING_ENERGY_DATA_HPP
#define SEAM_CARVING_ENERGY_DATA_HPP

// project
#include <seam_carving/tests/matrix_data.hpp>
#include <seam_carving/tests/test_data.hpp>

// 3rd party
#include <nlohmann/json.hpp>
#include <opencv2/core.hpp>
#include <gtest/gtest.h>

// std
#include <fstream>
#include <sstream>

namespace seam_carving::tests {
    struct EnergyData : TestData {
        cv::Mat sobel_matrix;
        cv::Mat vertical_map_matrix, horizontal_map_matrix;
    };

    void from_json(const nlohmann::json& j, EnergyData& energy_data);
    class EnergyTest : public testing::TestWithParam<EnergyData> { };
}

#endif //SEAM_CARVING_ENERGY_DATA_HPP

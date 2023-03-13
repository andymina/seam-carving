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
#include <gtest/gtest.h>

namespace seam_carving::tests {
    struct EnergyData : public TestData {
        cv::Mat sobel_matrix;
        cv::Mat vertical_map_matrix, horizontal_map_matrix;
    };

    class EnergyTest : public testing::TestWithParam<EnergyData> { };
}

#endif //SEAM_CARVING_ENERGY_DATA_HPP

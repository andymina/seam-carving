//
// Created by Andy Mina on 3/7/23.
//

// project
#include <seam_carving/energy.hpp>
#include <seam_carving/tests/test_data.hpp>
#include <seam_carving/tests/energy_data.hpp>

// 3rd party
#include <gtest/gtest.h>

// std
#include <iostream>

namespace sce = seam_carving::energy;
namespace sct = seam_carving::tests;
using sct::EnergyTest;

INSTANTIATE_TEST_SUITE_P(
    EnergyFromJSON,
    EnergyTest,
    testing::ValuesIn(
        sct::GetData<sct::EnergyData>(sct::kJSONDataFile)
    )
);

TEST_P(EnergyTest, ComputeVerticalMapReturnsCorrectValue) {
    sct::EnergyData energy_data = GetParam();

    cv::Mat input = energy_data.sobel_matrix;
    input.convertTo(input, CV_8U);
    cv::Mat expected = energy_data.vertical_map_matrix;
    expected.convertTo(expected, CV_16U);
    cv::Mat actual;

    sce::ComputeVerticalMap(input, actual);

    std::cout << expected << "\n\n";
    std::cout << actual << "\n\n";

    EXPECT_TRUE(sct::equalMatrices(expected, actual));
}

TEST_P(EnergyTest, ComputeHorizontalMapReturnsCorrectValue) {
    sct::EnergyData energy_data = GetParam();

    cv::Mat input = energy_data.sobel_matrix;
    input.convertTo(input, CV_8U);
    cv::Mat expected = energy_data.horizontal_map_matrix;
    expected.convertTo(expected, CV_16U);
    cv::Mat actual;

    sce::ComputeHorizontalMap(input, actual);

//    std::cout << expected << "\n\n";
//    std::cout << actual << "\n\n";

    EXPECT_TRUE(sct::equalMatrices(expected, actual)) << expected;
}

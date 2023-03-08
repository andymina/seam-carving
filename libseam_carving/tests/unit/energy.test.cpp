//
// Created by Andy Mina on 3/7/23.
//

// project
#include <seam_carving/energy.hpp>
#include <seam_carving/tests/test_data.hpp>
#include <seam_carving/tests/energy_data.hpp>


// 3rd party
#include <gtest/gtest.h>

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

TEST_P(EnergyTest, ComputeEnergyReturnsCorrectValue) {
    sct::EnergyData energy_data = GetParam();

    const cv::Mat& input = energy_data.original_matrix;
    const cv::Mat& expected = energy_data.sobel_matrix;
    cv::Mat actual;

    sce::ComputeEnergy(input, actual);

    // convert expected depth to match actual depth
    expected.convertTo(expected, CV_8U);

    EXPECT_TRUE(sct::equalMatrices(expected, actual));
}

//TEST_P(EnergyTest, ComputeVerticalMapReturnsCorrectValue) {
//    sct::EnergyData energy_data = GetParam();
//
//    const cv::Mat& input = energy_data.original_matrix;
//    const cv::Mat& expected = energy_data.vertical_map_matrix;
//    cv::Mat actual;
//    sce::ComputeEnergy()
//
//}

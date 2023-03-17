//
// Created by Andy Mina on 3/7/23.
//

// project
#include <seam_carving/energy.hpp>
#include <seam_carving/tests/matrix_utils.hpp>
#include <seam_carving/tests/print_utils.hpp>
#include <seam_carving/tests/json_utils.hpp>
#include <seam_carving/tests/energy_data.hpp>

// 3rd party
#include <gtest/gtest.h>

namespace sce = seam_carving::energy;
namespace sct = seam_carving::tests;
using sct::EnergyTest;

INSTANTIATE_TEST_SUITE_P(
    EnergyDataFromJSON,
    EnergyTest,
    testing::ValuesIn(
        sct::GetData<sct::EnergyData>(sct::kJSONDataFile)
    )
);

TEST_P(EnergyTest, ComputeVerticalMapReturnsCorrectValue) {
    sct::EnergyData energy_data = GetParam();

    cv::Mat input = energy_data.sobel_matrix;
    cv::Mat expected = energy_data.vertical_map_matrix;
    cv::Mat actual;

    sce::ComputeVerticalMap(input, actual);

    EXPECT_TRUE(sct::equalMatrices(expected, actual))
                        << "TestId - " << energy_data.test_id << "\n"
                        << sct::PrintWithLabel(expected, "expected") << "\n"
        << sct::PrintWithLabel(actual, "actual");
}

TEST_P(EnergyTest, ComputeHorizontalMapReturnsCorrectValue) {
    sct::EnergyData energy_data = GetParam();

    cv::Mat input = energy_data.sobel_matrix;
    cv::Mat expected = energy_data.horizontal_map_matrix;
    cv::Mat actual;

    sce::ComputeHorizontalMap(input, actual);

    EXPECT_TRUE(sct::equalMatrices(expected, actual))
        << "TestId - " << energy_data.test_id << "\n"
        << sct::PrintWithLabel(expected, "expected") << "\n"
        << sct::PrintWithLabel(actual, "actual");
}

//
// Created by Andy Mina on 3/7/23.
//

// project
#include <seam_carving/carver.hpp>
#include <seam_carving/seam.hpp>

#include <seam_carving/tests/print_utils.hpp>
#include <seam_carving/tests/json_utils.hpp>
#include <seam_carving/tests/carver_data.hpp>
#include <seam_carving/tests/dummy_carver.hpp>

// 3rd party
#include <gtest/gtest.h>

// stl
#include <iostream>

namespace sc = seam_carving;
namespace sct = seam_carving::tests;
using sct::CarverTest;

INSTANTIATE_TEST_SUITE_P(
    CarverDataFromJSON,
    CarverTest,
    testing::ValuesIn(
        sct::GetData<sct::CarverData>(sct::kJSONDataFile)
    )
);

TEST_P(CarverTest, FindVerticalSeamReturnsCorrectSeam) {
    sct::CarverData carver_data = GetParam();
    sct::DummyCarver carver;

    cv::Mat input = carver_data.original_matrix;
    sc::Seam expected = carver_data.vertical_seam;

    sc::Seam actual = carver.FindVerticalSeam(input);

    /** @TODO(#58): add printing for TestData and children */
    EXPECT_EQ(expected, actual)
       << "TestId - " << carver_data.test_id;
}

TEST_P(CarverTest, FindHorizontalSeamReturnsCorrectSeam) {
    sct::CarverData carver_data = GetParam();
    sct::DummyCarver carver;

    cv::Mat input = carver_data.original_matrix;
    sc::Seam expected = carver_data.horizontal_seam;

    sc::Seam actual = carver.FindHorizontalSeam(input);

    EXPECT_EQ(expected, actual)
        << "TestId - " << carver_data.test_id;
}



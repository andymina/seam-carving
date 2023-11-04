//
// Created by Andy Mina on 3/7/23.
//

// test
#include "print_utils.hpp"
#include "matrix_utils.hpp"
#include "json_utils.hpp"
#include "carver_data.hpp"

// project
#include <seam_carving/carver.hpp>
#include <seam_carving/seam.hpp>

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

TEST_P(CarverTest, FindVerticalSeams) {
    sct::CarverData carver_data = GetParam();

    cv::Mat input = carver_data.original_matrix;
    sc::Seam expected = carver_data.vertical_seam;

    sc::Seam actual = sc::Carver::FindVerticalSeams(1, input)[0];

    EXPECT_EQ(expected, actual)
        << sct::PrintWithLabel(expected, "expected") << "\n"
        << sct::PrintWithLabel(actual, "actual");
}

TEST_P(CarverTest, FindHorizontalSeams) {
    sct::CarverData carver_data = GetParam();

    cv::Mat input = carver_data.original_matrix;
    sc::Seam expected = carver_data.horizontal_seam;

    sc::Seam actual = sc::Carver::FindHorizontalSeams(1, input)[0];

    EXPECT_EQ(expected, actual)
        << sct::PrintWithLabel(expected, "expected") << "\n"
        << sct::PrintWithLabel(actual, "actual");
}

TEST_P(CarverTest, RemoveVerticalSeams) {
    sct::CarverData carver_data = GetParam();

    cv::Mat input = carver_data.original_matrix;
    cv::Mat expected = carver_data.remove_vertical_matrix;
    cv::Mat actual;

    sc::Seam seam = carver_data.vertical_seam;
    sc::Carver::RemoveVerticalSeams(1, input, actual);

    EXPECT_TRUE(sct::equalMatrices(expected, actual))
        << sct::PrintWithLabel(expected, "expected") << "\n"
        << sct::PrintWithLabel(actual, "actual");
}

TEST_P(CarverTest, RemoveHorizontalSeams) {
    sct::CarverData carver_data = GetParam();

    cv::Mat input = carver_data.original_matrix;
    cv::Mat expected = carver_data.remove_horizontal_matrix;
    cv::Mat actual;

    sc::Seam seam = carver_data.horizontal_seam;
    sc::Carver::RemoveHorizontalSeams(1, input, actual);

    EXPECT_TRUE(sct::equalMatrices(expected, actual))
        << sct::PrintWithLabel(expected, "expected") << "\n"
        << sct::PrintWithLabel(actual, "actual");
}

TEST_P(CarverTest, InsertVerticalSeam) {
    sct::CarverData carver_data = GetParam();

    cv::Mat input = carver_data.original_matrix;
    cv::Mat expected = carver_data.insert_vertical_matrix;
    cv::Mat actual;

    sc::Seam seam = carver_data.vertical_seam;
    sc::Carver::InsertVerticalSeams(1, input, actual);

    EXPECT_TRUE(sct::equalMatrices(expected, actual))
        << sct::PrintWithLabel(expected, "expected") << "\n"
        << sct::PrintWithLabel(actual, "actual");
}

TEST_P(CarverTest, InsertHorizontalSeam) {
    sct::CarverData carver_data = GetParam();

    cv::Mat input = carver_data.original_matrix;
    cv::Mat expected = carver_data.insert_horizontal_matrix;
    cv::Mat actual;

    sc::Seam seam = carver_data.horizontal_seam;
    sc::Carver::InsertHorizontalSeams(1, input, actual);

    EXPECT_TRUE(sct::equalMatrices(expected, actual))
        << sct::PrintWithLabel(expected, "expected") << "\n"
        << sct::PrintWithLabel(actual, "actual");
}

TEST_P(CarverTest, Carve) {
    sct::CarverData carver_data = GetParam();
    cv::Mat input = carver_data.original_matrix;
    cv::Mat actual;

    /** #CASE: expand one column */
    cv::Mat expected = carver_data.insert_vertical_matrix;
    sc::Carver::Carve(input.rows, input.cols + 1, input, actual);
    EXPECT_TRUE(sct::equalMatrices(expected, actual))
        << "#CASE: expand one column\n"
        << sct::PrintWithLabel(expected, "expected") << "\n"
        << sct::PrintWithLabel(actual, "actual");

    /** #CASE: shrink one column */
    expected = carver_data.remove_vertical_matrix;
    sc::Carver::Carve(input.rows, input.cols - 1, input, actual);
    EXPECT_TRUE(sct::equalMatrices(expected, actual))
        << "#CASE: shrink one column\n"
        << sct::PrintWithLabel(expected, "expected") << "\n"
        << sct::PrintWithLabel(actual, "actual");

    /** #CASE: expand one row */
    expected = carver_data.insert_horizontal_matrix;
    sc::Carver::Carve(input.rows + 1, input.cols, input, actual);
    EXPECT_TRUE(sct::equalMatrices(expected, actual))
        << "#CASE: expand one row\n"
        << sct::PrintWithLabel(expected, "expected") << "\n"
        << sct::PrintWithLabel(actual, "actual");

    /** #CASE: shrink one row */
    expected = carver_data.remove_horizontal_matrix;
    sc::Carver::Carve(input.rows - 1, input.cols, input, actual);
    EXPECT_TRUE(sct::equalMatrices(expected, actual))
        << "#CASE: shrink one row\n"
        << sct::PrintWithLabel(expected, "expected") << "\n"
        << sct::PrintWithLabel(actual, "actual");
}

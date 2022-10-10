#include "energy.h"

#include <doctest/doctest.h>
#include <opencv2/core.hpp>

#include <iostream>
#include <vector>

using std::cout;
using std::vector;
namespace nrg = SeamCarving::Energy;

vector<uchar> sample_input = {
    1, 8, 8, 3, 5, 4,
    7, 8, 1, 0, 8, 4,
    8, 0, 4, 7, 2, 9,
    9, 0, 0, 5, 9, 4,
    2, 4, 0, 2, 4, 5,
    2, 4, 2, 5, 3, 0
};

vector<ushort> sample_output = {
    10, 11, 11, 6, 11, 17,
    9, 10, 3, 6, 17, 13,
    10, 2, 6, 9, 9, 17,
    13, 2, 2, 7, 13, 8,
    4, 6, 2, 4, 4, 5,
    2, 4, 2, 5, 3, 0,
};

TEST_SUITE("energy.cpp") {
    TEST_CASE("ComputeVerticalEnergyMap function") {
        // setup
        cv::Mat input(6, 6, CV_8U, sample_input.data());
        cv::Mat expected(6, 6, CV_16U, sample_output.data());
        cv::Mat actual;

        // tests
        nrg::ComputeVerticalEnergyMap(input, actual);

        // compare
        input.convertTo(input, CV_16U);
        cv::Mat diff = actual != expected;
        bool equal_matrices = cv::countNonZero(diff) == 0;
        CHECK(equal_matrices);
    }

    TEST_CASE("ComputeHoriztonalEnergyMap function") {
        cv::Mat input(6, 6, CV_8U, sample_input.data());
        cv::transpose(input, input);
        cv::Mat expected(6, 6, CV_16U, sample_output.data());
        cv::transpose(expected, expected);
        cv::Mat actual;

        // tests
        nrg::ComputeHorizontalEnergyMap(input, actual);

        // compare
        input.convertTo(input, CV_16U);
        cv::Mat diff = actual != expected;
        bool equal_matrices = cv::countNonZero(diff) == 0;
        CHECK(equal_matrices);
    }
}
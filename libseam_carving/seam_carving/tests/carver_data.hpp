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
#include <gtest/gtest.h>

namespace seam_carving::tests {
    struct CarverData : TestData {
        Seam vertical_seam, horizontal_seam;
        cv::Mat remove_vertical_matrix, remove_horizontal_matrix;
        cv::Mat insert_vertical_matrix, insert_horizontal_matrix;
    };


    class CarverTest : public testing::TestWithParam<CarverData> { };
}

#endif //SEAM_CARVING_CARVER_DATA_HPP

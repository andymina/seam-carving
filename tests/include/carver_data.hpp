//
// Created by Andy Mina on 3/7/23.
//

#ifndef SEAM_CARVING_CARVER_DATA_HPP
#define SEAM_CARVING_CARVER_DATA_HPP

// project
#include <seam_carving/seam.hpp>

// test
#include "test_data.hpp"
#include "print_utils.hpp"

// 3rd party
#include <nlohmann/json.hpp>
#include <opencv2/core.hpp>
#include <gtest/gtest.h>

namespace seam_carving::tests {
    class CarverData : public TestData {
    public:
        Seam vertical_seam, horizontal_seam;
        cv::Mat remove_vertical_matrix, remove_horizontal_matrix;
        cv::Mat insert_vertical_matrix, insert_horizontal_matrix;

        inline friend std::ostream& operator<<(std::ostream& os, const CarverData& data) {
            os << "TestId - " << data.test_id << "\n";
            os << PrintWithLabel(data.vertical_seam, "vertical seam") << "\n";
            os << PrintWithLabel(data.horizontal_seam, "horizontal seam") << "\n";

            os << PrintWithLabel(data.remove_vertical_matrix, "remove vertical matrix") << "\n";
            os << PrintWithLabel(data.remove_horizontal_matrix, "remove horizontal matrix") << "\n";

            os << PrintWithLabel(data.insert_vertical_matrix, "insert vertical matrix") << "\n";
            os << PrintWithLabel(data.insert_horizontal_matrix, "insert horizontal matrix");
            return os;
        }
    };


    class CarverTest : public testing::TestWithParam<CarverData> { };
}

#endif //SEAM_CARVING_CARVER_DATA_HPP

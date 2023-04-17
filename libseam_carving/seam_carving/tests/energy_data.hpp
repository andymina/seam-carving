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
    class EnergyData : public TestData {
    public:
        cv::Mat sobel_matrix;
        cv::Mat vertical_map_matrix, horizontal_map_matrix;

        inline friend std::ostream& operator<<(std::ostream& os, const EnergyData& data) {
            os << PrintWithLabel(data.sobel_matrix, "sobel matrix") << "\n";
            os << PrintWithLabel(data.vertical_map_matrix, "vertical map") << "\n";
            os << PrintWithLabel(data.horizontal_map_matrix, "horizontal map");
            return os;
        }
    };

    class EnergyTest : public testing::TestWithParam<EnergyData> { };
}

#endif //SEAM_CARVING_ENERGY_DATA_HPP

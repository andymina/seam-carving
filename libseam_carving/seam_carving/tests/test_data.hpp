//
// Created by Andy Mina on 3/7/23.
//

#ifndef SEAM_CARVING_TEST_DATA_HPP
#define SEAM_CARVING_TEST_DATA_HPP

// 3rd party
#include <nlohmann/json.hpp>
#include <opencv2/core.hpp>

namespace seam_carving {
    struct TestData {
        std::string test_id;
        cv::Mat original_matrix;
    };

    inline void from_json(const nlohmann::json& j, TestData& test_data) {
        test_data.test_id = j.at("test_id");
        test_data.original_matrix = j.at("input");
    }
}

#endif //SEAM_CARVING_TEST_DATA_HPP

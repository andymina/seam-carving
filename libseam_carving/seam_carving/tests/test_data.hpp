//
// Created by Andy Mina on 3/7/23.
//

#ifndef SEAM_CARVING_TEST_DATA_HPP
#define SEAM_CARVING_TEST_DATA_HPP

// project
#include <seam_carving/tests/matrix_data.hpp>

// 3rd party
#include <nlohmann/json.hpp>
#include <opencv2/core.hpp>

// std
#include <fstream>
#include <vector>

namespace seam_carving::tests {
    struct TestData {
        std::string test_id;
        cv::Mat original_matrix;
    };

    void from_json(const nlohmann::json& j, TestData& test_data);

    template<typename T>
    std::vector<T> GetData(const std::string& path);

    /** Path to JSON data file for tests from libseam_carving/ */
    const std::string json_data_file = "tests/data.json";
}

#endif //SEAM_CARVING_TEST_DATA_HPP

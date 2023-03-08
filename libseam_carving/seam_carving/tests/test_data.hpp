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
#include <sstream>
#include <vector>

namespace seam_carving::tests {
    struct TestData {
        int test_id;
        cv::Mat original_matrix;
    };

    inline void from_json(const nlohmann::json& j, TestData& test_data) {
        test_data.test_id = j.at("test_id");
        test_data.original_matrix = j.at("input");
    }

    template<typename T>
    inline std::vector<T> GetData(const std::string& path) {
        std::ifstream file(path);
        nlohmann::json j = nlohmann::json::parse(file);
        return j;
    }

    inline std::string PrintToString(const TestData& test_data) {
        std::stringstream ss;
        ss << ">\n";
        ss << "- Test Id: " << test_data.test_id << "\n\n";
        ss << "- Original Matrix\n" << test_data.original_matrix << "\n\n";
        ss << "\n";
        return ss.str();
    };

    /** Path to JSON data file for tests from libseam_carving/ */
    const std::string kJSONDataFile = "tests/data.json";
}

#endif //SEAM_CARVING_TEST_DATA_HPP

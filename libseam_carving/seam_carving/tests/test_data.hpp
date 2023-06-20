//
// Created by Andy Mina on 3/7/23.
//

#ifndef SEAM_CARVING_TEST_DATA_HPP
#define SEAM_CARVING_TEST_DATA_HPP

// project
#include <seam_carving/tests/print_utils.hpp>

// 3rd party
#include <nlohmann/json.hpp>
#include <opencv2/core.hpp>

// std
#include <fstream>
#include <vector>
#include <iostream>

namespace seam_carving::tests {
    /** Path to JSON data file for tests from libseam_carving/ */
    const std::string kJSONDataFile = "tests/data.json";

    template<typename T>
    inline std::vector<T> GetData(const std::string& path) {
        std::ifstream file(path);
        nlohmann::json j = nlohmann::json::parse(file);
        file.close();
        return j;
    }

    class TestData {
    public:
        int test_id;
        cv::Mat original_matrix;

        inline friend std::ostream& operator<<(std::ostream& os, const TestData& data) {
            os << "TestId - " << data.test_id << "\n";
            os << PrintWithLabel(data.original_matrix, "original matrix");
            return os;
        }
    };
}

#endif //SEAM_CARVING_TEST_DATA_HPP

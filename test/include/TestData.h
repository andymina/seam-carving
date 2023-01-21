#include "MatrixJSON.h"

#include <opencv2/core.hpp>
#include <nlohmann/json.hpp>

namespace SeamCarving {
    struct TestData {
        std::string test_id;
        cv::Mat original_matrix;
	};

    inline void from_json(const nlohmann::json& j, TestData& test_data) {
        test_data.test_id = j.at("test_id");
        test_data.original_matrix = j.at("input");
    }
}
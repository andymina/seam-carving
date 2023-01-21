#include "Seam.h"
#include "TestData.h"

#include <opencv2/core.hpp>

namespace SeamCarving {
    struct CarverTestData : TestData {
        Seam vertical_seam, horiztonal_seam;        
        cv::Mat remove_vertical_matrix, remove_horizontal_matrix;
        cv::Mat insert_vertical_matrix, insert_horizontal_matrix;        
    };

    inline void from_json(const nlohmann::json& j, CarverTestData& carver_data) {
        const nlohmann::json& root = j.at("carver");
        
        carver_data.vertical_seam = root.at("find - vertical");
        carver_data.horiztonal_seam =  root.at("find - horizontal");

        carver_data.remove_vertical_matrix = root.at("remove - vertical");
        carver_data.remove_horizontal_matrix = root.at("remove - horizontal");
        carver_data.insert_vertical_matrix = root.at("insert - vertical");
        carver_data.insert_horizontal_matrix = root.at("insert - horizontal");
    }
}

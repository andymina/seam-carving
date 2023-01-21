#include "TestData.h"

#include <opencv2/core.hpp>

namespace SeamCarving {
    struct EnergyTestData : TestData {
		cv::Mat sobel_matrix;
        cv::Mat vertical_map_matrix, horizontal_map_matrix;
	};

    inline void from_json(const nlohmann::json& j, EnergyTestData& energy_data) {
        const nlohmann::json& root = j.at("energy");

        energy_data.sobel_matrix = root.at("sobel");

        energy_data.vertical_map_matrix = root.at("map - vertical");
        energy_data.horizontal_map_matrix = root.at("map - horizontal");
    }
}
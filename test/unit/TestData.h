#include "constants.h"

#include <opencv2/core.hpp>
#include <nlohmann/json.hpp>

namespace SeamCarving {
    struct TestData {
        std::string test_id;
        cv::Mat original_matrix;
	};

	struct EnergyTestData : TestData {
		cv::Mat sobel_matrix, vertical_map_matrix, horizontal_map_matrix;
	};

	struct CarverTestData : TestData {
		Seam vertical_seam, horiztonal_seam;		
        cv::Mat insert_vertical_matrix, insert_horizontal_matrix;
        cv::Mat remove_vertical_matrix, remove_horizontal_matrix;
    };
}

NLOHMANN_JSON_NAMESPACE_BEGIN
template<>
struct adl_serializer<SeamCarving::TestData> {
	/**
	 * Reads in a matrix from JSON. Mainly used for reading in test matrices.
	 * 
	 * @remarks the matrix read in will be <int>
	*/
	static void from_json(const nlohmann::json& j, SeamCarving::TestData& test_data) {
		test_data.test_id = j["test_id"];
		test_data.original_matrix = j["input"];

		test_data.sobel_matrix = j["energy"]["sobel"];
		test_data.vertical_map_matrix = j["energy"]["map - vertical"];
		test_data.horizontal_map_matrix = j["energy"]["map - horizontal"];

		test_data.vertical_seam = j[""]
	}
};
NLOHMANN_JSON_NAMESPACE_END
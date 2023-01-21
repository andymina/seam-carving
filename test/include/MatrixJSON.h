#include <opencv2/core.hpp>
#include <nlohmann/json.hpp>

#include <vector>

NLOHMANN_JSON_NAMESPACE_BEGIN
template<>
struct adl_serializer<cv::Mat> {
    static void from_json(const nlohmann::json& j, cv::Mat& matrix) {
        int rows = j.at("rows"), cols = j.at("cols");
        std::vector<int> data = j.at("data");
        matrix = cv::Mat(rows, cols, CV_32S, data.data());
    }
};
NLOHMANN_JSON_NAMESPACE_END
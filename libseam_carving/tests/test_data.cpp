//
// Created by Andy Mina on 3/7/23.
//

#include <seam_carving/tests/test_data.hpp>

namespace seam_carving::tests {
    void from_json(const nlohmann::json& j, TestData& test_data) {
        test_data.test_id = j.at("test_id");
        test_data.original_matrix = j.at("input");
    }

    template<typename T>
    std::vector<T> GetData(const std::string& path) {
        std::ifstream file(path);
        nlohmann::json j;
        file >> j;
        return j;
    }
}

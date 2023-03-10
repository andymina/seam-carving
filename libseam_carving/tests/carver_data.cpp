//
// Created by Andy Mina on 3/9/23.
//

#include <seam_carving/tests/carver_data.hpp>

namespace seam_carving::tests {
    void from_json(const nlohmann::json& j, CarverData& carver_data) {
        nlohmann::from_json(j, static_cast<TestData&>(carver_data));

        const nlohmann::json& root = j.at("carver");
        carver_data.vertical_seam = root.at("find - vertical");
        carver_data.horizontal_seam =  root.at("find - horizontal");

        carver_data.remove_vertical_matrix = root.at("remove - vertical");
        carver_data.remove_horizontal_matrix = root.at("remove - horizontal");
        carver_data.insert_vertical_matrix = root.at("insert - vertical");
        carver_data.insert_horizontal_matrix = root.at("insert - horizontal");
    }
}

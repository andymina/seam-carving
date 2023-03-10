//
// Created by Andy Mina on 3/7/23.
//

#include <seam_carving/tests/energy_data.hpp>

namespace seam_carving::tests {
    void from_json(const nlohmann::json& j, EnergyData& energy_data) {
        nlohmann::from_json(j, static_cast<TestData&>(energy_data));

        const nlohmann::json& root = j.at("energy");
        energy_data.sobel_matrix = root.at("sobel");
        energy_data.vertical_map_matrix = root.at("map - vertical");
        energy_data.horizontal_map_matrix = root.at("map - horizontal");
    }
}

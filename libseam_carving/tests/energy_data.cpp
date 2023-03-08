//
// Created by Andy Mina on 3/7/23.
//

#include <seam_carving/tests/energy_data.hpp>

namespace seam_carving::tests {
    void from_json(const nlohmann::json& j, EnergyData& energy_data) {
        const nlohmann::json& root = j.at("energy");

        energy_data.sobel_matrix = root.at("sobel");

        energy_data.vertical_map_matrix = root.at("map - vertical");
        energy_data.horizontal_map_matrix = root.at("map - horizontal");
    }

    std::string PrintToString(const EnergyData& energy_data) {
        std::stringstream ss;
        ss << ">\n";
        ss << "- Sobel\n" << energy_data.sobel_matrix << "\n\n";
        ss << "- Vertical Map\n" << energy_data.vertical_map_matrix << "\n\n";
        ss << "- Horizontal Map\n" << energy_data.horizontal_map_matrix << "\n";
        ss << "\n";
        return ss.str();
    }
}

//
// Created by Andy Mina on 3/11/23.
//

// project
#include "seam_carving/tests/json_utils.hpp"

namespace seam_carving {
    void to_json(nlohmann::json& j, const Coord& coord) {
        j = nlohmann::json{
            {"row", coord.row()},
            {"col", coord.col()}
        };
    }

    void from_json(const nlohmann::json& j, Coord& coord) {
        coord = Coord(j.at("row"), j.at("col"));
    }

    void to_json(nlohmann::json& j, const Seam& seam) {
        j = nlohmann::json{
            {"dir", seam.dir()},
            {"coords", seam.coords()}
        };
    }

    void from_json(const nlohmann::json& j, Seam& seam) {
        seam = Seam(j.at("dir"), j.at("coords"));
    }
}

namespace seam_carving::tests {
    void from_json(const nlohmann::json& j, TestData& test_data) {
        test_data.test_id = j.at("test_id");
        test_data.original_matrix = j.at("input");
    }

    void from_json(const nlohmann::json& j, EnergyData& energy_data) {
        nlohmann::from_json(j, static_cast<TestData&>(energy_data));

        const nlohmann::json& root = j.at("energy");
        energy_data.sobel_matrix = root.at("sobel");
        energy_data.vertical_map_matrix = root.at("map - vertical");
        energy_data.horizontal_map_matrix = root.at("map - horizontal");
    }

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

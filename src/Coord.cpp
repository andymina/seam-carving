#include "Coord.h"

namespace SeamCarving {
    Coord::Coord(const int& row, const int& col):
        row_{row}, col_{col}
        { }
    
    bool operator==(const Coord& lhs, const Coord& rhs) {
        return (lhs.row() == rhs.row()) && (lhs.col() == rhs.col());
    }

    void to_json(nlohmann::json& j, const Coord& coord) {
        j = nlohmann::json{
            {"row", coord.row()},
            {"col", coord.col()}
        };
    }

    void from_json(const nlohmann::json& j, Coord& coord) {
        coord = Coord(j.at("row"), j.at("col"));
    }
}
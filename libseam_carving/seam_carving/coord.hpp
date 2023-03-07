//
// Created by Andy Mina on 3/7/23.
//

#ifndef SEAM_CARVING_COORD_HPP
#define SEAM_CARVING_COORD_HPP

// 3rd party
#include <nlohmann/json.hpp>

namespace seam_carving {
    /** TODO(andymina): document this */
    class Coord {
    private:
        int row_, col_;

    public:
        explicit Coord(const int& row = 0, const int& col = 0);

        inline const int& row() const { return row_; };
        inline const int& col() const { return col_; };

        /**
         * Determines if two Coords are equal based on their rows and cols.
         *
         * @param rhs - the coord to compare
         * @returns true if the Coords are equal, otherwise false
        */
        friend bool operator==(const Coord& lhs, const Coord& rhs);
    };

    void to_json(nlohmann::json& j, const Coord& coord);
    void from_json(const nlohmann::json& j, Coord& coord);
}

#endif //SEAM_CARVING_COORD_HPP

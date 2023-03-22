//
// Created by Andy Mina on 3/7/23.
//

#ifndef SEAM_CARVING_COORD_HPP
#define SEAM_CARVING_COORD_HPP

// 3rd party
#include <nlohmann/json.hpp>

// stl
#include <ostream>

namespace seam_carving {
    /** TODO(andymina): document this */
    class Coord {
    private:
        int row_, col_;

    public:
        inline explicit Coord(const int& row = 0, const int& col = 0):
            row_{row}, col_{col}
            { }

        inline const int& row() const { return row_; };
        inline const int& col() const { return col_; };

        /**
         * Determines if two Coords are equal based on their rows_ and cols_.
         *
         * @param rhs - the coord to compare
         * @returns true if the Coords are equal, otherwise false
        */
        inline friend bool operator==(const Coord& lhs, const Coord& rhs) {
            return (lhs.row() == rhs.row()) && (lhs.col() == rhs.col());
        }

        inline friend std::ostream& operator<<(std::ostream& os, const Coord& coord) {
            os << "(" << coord.row() << ", " << coord.col() << ")";
            return os;
        }
    };
}

#endif //SEAM_CARVING_COORD_HPP

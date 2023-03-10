//
// Created by Andy Mina on 3/7/23.
//

#ifndef SEAM_CARVING_SEAM_HPP
#define SEAM_CARVING_SEAM_HPP

// project
#include <seam_carving/coord.hpp>

// 3rd party
#include <nlohmann/json.hpp>

// stl
#include <vector>
#include <sstream>
#include <ostream>

namespace seam_carving {
    /** A Enum to help better describe what direction to use in functions and seams. */
    enum Dir { VERT, HORZ };
    // define to_json/from_json
    NLOHMANN_JSON_SERIALIZE_ENUM(Dir, {
        { VERT, "VERT" },
        { HORZ, "HORZ" }
    });

    /**
     * Helper class to describe a seam.
     *
     * TODO(andymina): document this class and its functions
    */
    class Seam {
    private:
        Dir dir_;
        std::vector<Coord> coords_;

    public:
        /**
         * Constructs a Seam using the specified data.
         *
         * @param dir - the direction of the seam
         * @param data - a vector of the pixels in the seam specified by Coord
        */
        explicit Seam(const Dir& dir = VERT, const std::vector<Coord>& coords = {});

        inline const Dir& dir() const { return dir_; };
        inline const std::vector<Coord>& coords() const { return coords_; };
        inline size_t size() const { return coords_.size(); }
        inline Coord& operator[](std::size_t idx) { return coords_[idx]; };
        inline const Coord& operator[](std::size_t idx) const { return coords_[idx]; };

        void push(const Coord& coord);

        /**
         * Determines if two seams are equivalent based on their direction, size, and data vectors.
         *
         * @param rhs - the other Seam
         * @returns true if the seams are equal, false otherwise
        */
        inline friend bool operator==(const Seam& lhs, const Seam& rhs) {
            return (lhs.dir() == rhs.dir()) &&
                   (lhs.size() == rhs.size()) &&
                   (lhs.coords() == rhs.coords());
        }

        inline friend std::ostream& operator<<(std::ostream& os, const Seam& seam) {
            for (std::size_t i = 0; i < seam.size(); i++) {
                os << seam[i];
                if (i != seam.size() - 1)
                    os << ", ";
            }
            return os;
        }
    };

    void to_json(nlohmann::json& j, const Seam& seam);
    void from_json(const nlohmann::json& j, Seam& seam);

    std::string PrintToString(const Seam& seam, const std::string& label = "");
}

#endif //SEAM_CARVING_SEAM_HPP

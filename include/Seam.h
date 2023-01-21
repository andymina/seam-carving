#ifndef Seam_h
#define Seam_h

// project
#include "Coord.h"

// 3rd party
#include <nlohmann/json.hpp>

// std
#include <vector>

namespace SeamCarving {
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
            Seam(const Dir& dir = VERT, const std::vector<Coord>& coords = {});

            inline const Dir& dir() const { return dir_; };
            inline const std::vector<Coord>& coords() const { return coords_; };
            inline Coord& operator[](int idx) { return coords_[idx]; };
            inline const Coord& operator[](int idx) const { return coords_[idx]; };

            void pushCoord(const Coord& coord);



            /**
             * Determines if two seams are equivalent based on their direction, size, and data vectors.
             * 
             * @param rhs - the other Seam
             * @returns true if the seams are equal, false otherwise
            */
            friend bool operator==(const Seam& lhs, const Seam& rhs);  
    };

    void to_json(nlohmann::json& j, const Seam& seam);
    void from_json(const nlohmann::json& j, Seam& seam);
}

#endif /** Seam_h */
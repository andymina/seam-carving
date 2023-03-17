//
// Created by Andy Mina on 3/7/23.
//

#include <seam_carving/seam.hpp>

namespace seam_carving {
    Seam::Seam(const Dir& dir, const std::vector<Coord>& coords):
            dir_{dir}, coords_{coords}
    { }

    void Seam::push(const Coord& coord) {
        coords_.push_back(coord);
    }
}

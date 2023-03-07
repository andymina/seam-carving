//
// Created by Andy Mina on 3/7/23.
//

#include <seam_carving/seam.hpp>

namespace seam_carving {
    Seam::Seam(const Dir& dir, const std::vector<Coord>& coords):
            dir_{dir}, coords_{coords}
    { }

    void Seam::pushCoord(const Coord& coord) {
        coords_.push_back(coord);
    }

    bool operator==(const Seam& lhs, const Seam& rhs) {
        return (lhs.dir() == rhs.dir()) &&
               (lhs.coords().size() == rhs.coords().size()) &&
               (lhs.coords() == rhs.coords());
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
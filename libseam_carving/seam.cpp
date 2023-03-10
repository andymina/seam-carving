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

    void to_json(nlohmann::json& j, const Seam& seam) {
        j = nlohmann::json{
                {"dir", seam.dir()},
                {"coords", seam.coords()}
        };
    }

    void from_json(const nlohmann::json& j, Seam& seam) {
        seam = Seam(j.at("dir"), j.at("coords"));
    }

    std::string PrintToString(const Seam& seam, const std::string& label) {
        std::stringstream ss;
        ss << ">>> " << label << "\n" << seam;
        return ss.str();
    }
}

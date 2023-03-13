//
// Created by Andy Mina on 3/13/23.
//

#ifndef SEAM_CARVING_PRINT_UTILS_HPP
#define SEAM_CARVING_PRINT_UTILS_HPP

// project
#include <seam_carving/tests/print_utils.hpp>

namespace seam_carving::tests {
    template <typename T>
    inline std::string Print(const T& t, const std::string& label = "") {
        std::stringstream ss;
        ss << ">>> ";
        if (!label.empty()) {
            ss << label;
        }
        ss << "\n" << t;
        return ss.str();
    }
}

#endif //SEAM_CARVING_PRINT_UTILS_HPP

//
// Created by Andy Mina on 3/13/23.
//

#ifndef SEAM_CARVING_PRINT_UTILS_HPP
#define SEAM_CARVING_PRINT_UTILS_HPP

// test
#include "print_utils.hpp"

// stl
#include <sstream>

namespace seam_carving::tests {
    template <typename T>
    inline std::string PrintWithLabel(const T& t, const std::string& label = "") {
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

//
// Created by Andy Mina on 3/10/23.
//

#ifndef SEAM_CARVING_DUMMY_CARVER_HPP
#define SEAM_CARVING_DUMMY_CARVER_HPP

// project
#include <seam_carving/carver.hpp>

namespace seam_carving::tests {
    class DummyCarver : public Carver {
    public:
        DummyCarver() { };
    };
}

#endif //SEAM_CARVING_DUMMY_CARVER_HPP

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
        DummyCarver() = default;

        inline Seam FindVerticalSeam(cv::InputArray img) {
            return Carver::FindVerticalSeam(img);
        }

        inline Seam FindHorizontalSeam(cv::InputArray img) {
            return Carver::FindHorizontalSeam(img);
        }

        void Carve(cv::Mat &img, const int &target_rows, const int &target_cols) override { };
    };
}

#endif //SEAM_CARVING_DUMMY_CARVER_HPP

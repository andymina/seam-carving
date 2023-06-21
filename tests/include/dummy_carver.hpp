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
        inline static Seam FindVerticalSeam(cv::InputArray img) {
            return Carver::FindVerticalSeam(img);
        }

        inline static Seam FindHorizontalSeam(cv::InputArray img) {
            return Carver::FindHorizontalSeam(img);
        }

        inline static void RemoveVerticalSeam(const Seam& seam, cv::InputArray input, cv::OutputArray output) {
            Carver::RemoveVerticalSeam(seam, input, output);
        }

        inline static void RemoveHorizontalSeam(const Seam& seam, cv::InputArray input, cv::OutputArray output) {
            Carver::RemoveHorizontalSeam(seam, input, output);
        }

        inline static void InsertVerticalSeam(const Seam& seam, cv::InputArray input, cv::OutputArray output) {
            Carver::InsertVerticalSeam(seam, input, output);
        }

        inline static void InsertHorizontalSeam(const Seam &seam, cv::InputArray input, cv::OutputArray output) {
            Carver::InsertHorizontalSeam(seam, input, output);
        }
    };
}

#endif //SEAM_CARVING_DUMMY_CARVER_HPP

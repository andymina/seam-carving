//
// Created by Andy Mina on 3/7/23.
//

#ifndef SEAM_CARVING_CARVER_HPP
#define SEAM_CARVING_CARVER_HPP

// project
#include <seam_carving/energy.hpp>
#include <seam_carving/seam.hpp>

// 3rd party
#include <opencv2/core.hpp>

// std
#include <algorithm>
#include <limits>

namespace seam_carving {
    /**
     * Base class for all carvers
     */
    class Carver {
    public:
        /**
         * @param img the image to find the optimal seam in
         * @returns the optimal seam to be removed
         */
        static Seam FindVerticalSeam(cv::InputArray img);
        static Seam FindHorizontalSeam(cv::InputArray img);

        /**
         * @param seam the seam to be removed
         * @param img the target image
         */
        static void RemoveVerticalSeam(const Seam &seam, cv::InputArray input, cv::OutputArray output);
        static void RemoveHorizontalSeam(const Seam &seam, cv::InputArray input, cv::OutputArray output);

        /**
         * @param seam the seam to be inserted
         * @param img the target image
         */
        static void InsertVerticalSeam(const Seam &seam, cv::InputArray input, cv::OutputArray output);
        static void InsertHorizontalSeam(const Seam &seam, cv::InputArray input, cv::OutputArray output);

        /**
         * @param rows the target number of rows for the new image
         * @param cols the target number of cols for the new image
         * @param input the image to be carved
         * @param output a carved version of input
         */
        static void Carve(const int& rows, const int& cols, cv::InputArray input, cv::OutputArray output);
    };
}

#endif //SEAM_CARVING_CARVER_HPP

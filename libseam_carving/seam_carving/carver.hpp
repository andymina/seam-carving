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
    protected:
        /**
         * @param img the image to find the optimal seam in
         * @returns the optimal seam to be removed
         */
        Seam FindVerticalSeam(cv::InputArray img);
        Seam FindHorizontalSeam(cv::InputArray img);

        /**
         * @param seam the seam to be removed
         * @param img the target image
         */
        void RemoveVerticalSeam(const Seam &seam, cv::InputArray input, cv::OutputArray output);
        void RemoveHorizontalSeam(const Seam &seam, cv::InputArray input, cv::OutputArray output);

        /**
         * @param seam the seam to be inserted
         * @param img the target image
         */
        void InsertVerticalSeam(const Seam &seam, cv::InputArray input, cv::OutputArray output);
        void InsertHorizontalSeam(const Seam &seam, cv::InputArray input, cv::OutputArray output);

    public:
        /**
         * Carves the passed image to be the specified size. The target size can be larger
         * or smaller than the current size of the image in either dimension.
         *
         * All children classes must implement a default carving algorithm. Children classes can
         * overload this methods to create their own carvers. This default Carve method will modify the
         * image passed.
         *
         * @param img the source img; will be modified
         * @param target_rows the desired number of rows in the target image
         * @param target_cols the desired number of cols in the target image
         */
        virtual void Carve(cv::Mat &img, const int &target_rows, const int &target_cols) = 0;
    };
}

#endif //SEAM_CARVING_CARVER_HPP

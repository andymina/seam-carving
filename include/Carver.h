/**
 * Andy Mina
 */
#ifndef Carver_h
#define Carver_h

// project
#include "Seam.h"
#include "energy.h"

// 3rd party
#include <opencv2/core.hpp>

// std
#include <algorithm>
#include <limits>

namespace SeamCarving {
/**
 * Base class for all carvers
 */
class Carver {
   protected:
        /**
         * @param img the image to find the optimal seam in
         * @returns the optimal seam to be removed
         */
        Seam FindOptimalVerticalSeam(cv::InputArray img);
        Seam FindOptimalHorizontalSeam(cv::InputArray img);

        /**
         * @param seam the seam to be removed
         * @param img the target image
         */
        void RemoveVerticalSeam(const Seam &seam, cv::InputArray in_img, cv::OutputArray out_img);
        void RemoveHorizontalSeam(const Seam &seam, cv::InputArray in_img, cv::OutputArray out_img);

        /**
         * @param seam the seam to be inserted
         * @param img the target image
         */
        void InsertVerticalSeam(const Seam &seam, cv::InputArray in_img, cv::OutputArray out_img);
        void InsertHorizontalSeam(const Seam &seam, cv::InputArray in_img, cv::OutputArray out_img);

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
}  // namespace SeamCarving

#endif
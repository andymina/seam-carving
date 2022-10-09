/**
 * Andy Mina
 */
#ifndef Carver_h
#define Carver_h

// std
#include <algorithm>
#include <limits>

// 3rd party
#include <opencv2/core.hpp>

// utils
#include "constants.h"
#include "energy.h"

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
    Seam FindOptimalVerticalSeam(const Image &img);
    Seam FindOptimalHorizontalSeam(const Image &img);

    /**
     * @param seam the seam to be removed
     * @param img the target image
     */
    void RemoveVerticalSeam(const Seam &seam, Image &img);
    void RemoveHorizontalSeam(const Seam &seam, Image &img);

    /**
     * @param seam the seam to be inserted
     * @param img the target image
     */
    void InsertVerticalSeam(const Seam &seam, Image &img);
    void InsertHorizontalSeam(const Seam &seam, Image &img);

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
    virtual void Carve(Image &img, const int &target_rows, const int &target_cols) = 0;
};
}  // namespace SeamCarving

#endif
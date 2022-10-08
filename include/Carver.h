/**
 * Andy Mina
 */
#ifndef Carver_h
#define Carver_h

// std
#include <algorithm>

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
     * @returns the image with the specified seam removed
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
    virtual void Carve(const int &target_rows, const int &target_cols) = 0;
};
}  // namespace SeamCarving

#endif
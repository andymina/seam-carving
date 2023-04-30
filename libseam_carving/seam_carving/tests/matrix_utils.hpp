//
// Created by Andy Mina on 3/13/23.
//

#ifndef SEAM_CARVING_MATRIX_UTILS_HPP
#define SEAM_CARVING_MATRIX_UTILS_HPP

// 3rd party
#include <opencv2/core.hpp>

namespace seam_carving::tests {
    /**
     * @brief Checks if the lhs and the rhs matrices are identical in values. Adapted from
     * https://stackoverflow.com/a/32440830
     * @return true if the matrices are equal, false if not
     */
    bool equalMatrices(const cv::Mat lhs, const cv::Mat rhs) {
        if (lhs.dims != rhs.dims || lhs.size != rhs.size || lhs.elemSize() != rhs.elemSize())
            return false;

        if (lhs.isContinuous() && rhs.isContinuous())
            return memcmp(lhs.ptr(), rhs.ptr(), lhs.total() * lhs.elemSize()) == 0;

        const cv::Mat* arrays[] = {&lhs, &rhs, 0};
        uchar* ptrs[2];
        cv::NAryMatIterator it( arrays, ptrs, 2);
        for(unsigned int p = 0; p < it.nplanes; p++, ++it)
            if (memcmp( it.ptrs[0], it.ptrs[1], it.size * lhs.elemSize()) != 0)
                return false;

        return true;
    }
}

#endif //SEAM_CARVING_MATRIX_UTILS_HPP

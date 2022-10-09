/**
 * Andy Mina
 *
 * Defines the class to carry out seam carving operations: CarvableImage. Simplifies the
 * user-facing logic to carve seams out of an image or insert them.
 */
#ifndef CarvableImage_h
#define CarvableImage_h

#include <algorithm>
#include <cmath>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <vector>

#include "constants.h"
#include "energy.h"

namespace SeamCarving {
/**
 * Class that carries out seam carving operations.
 */
class CarvableImage {
   private:
    /** --- DATA MEMBERS --- */
    /**
     * this->original - stores the original image for comparison and if needed to reset
     * this->res_img - the resulting image after any seam operations
     * this->trans_img - the transposed image of this->res_img used for internal calculations
     */
    cv::Mat original, res_img, trans_img;
    int rows_, cols_;

    /** --- HELPERS --- */

    /**
     * @param img the image to find the optimal seam in
     * @returns the optimal seam to be removed
     */
    Seam __FindOptimalVerticalSeam(const cv::Mat &img);
    Seam __FindOptimalHorizontalSeam(const cv::Mat &img);

    /**
     * @param seam the seam to be removed
     * @param img the target image
     * @returns the image with the specified seam removed
     */
    cv::Mat __RemoveVerticalSeam(const Seam &seam, const cv::Mat &img);
    cv::Mat __RemoveHorizontalSeam(const Seam &seam, const cv::Mat &img);

    /**
     * @param seam the seam to be inserted
     * @param img the target image
     * @returns the image with the seam inserted
     */
    cv::Mat __InsertVerticalSeam(const Seam &seam, const cv::Mat &img);
    cv::Mat __InsertHorizontalSeam(const Seam &seam, const cv::Mat &img);

    std::vector<Seam> __FindKOptimalSeams(const int &k, const Dir &dir, cv::Mat &img);
    void __HighlightSeam(cv::Mat &img, const Seam &seam, const cv::Vec3b &color);

   public:
    /** Constructors */
    CarvableImage(const std::string &path);

    /** Getters */
    inline const cv::Mat &GetOriginal() const { return this->original; };
    inline const cv::Mat &GetResult() const { return this->res_img; };
    inline const cv::Mat &GetTranspose() const { return this->trans_img; };
    inline int num_rows() const { return this->rows_; };
    inline int num_cols() const { return this->cols_; };

    /** Seams */
    void SeamCarve(const int &target_rows, const int &target_cols);
    Seam FindOptimalSeam(const Dir &dir = VERT);
    std::vector<Seam> FindKOptimalSeams(const int &k, const Dir &dir = VERT);
    void RemoveSeam(const Dir &dir);
    void InsertSeam(const Dir &dir);
    void InsertKSeams(const int &k, const Dir &dir);

    /** Highlighting */
    void HighlightSeam(const Seam &seam, const int &r = 176, const int &g = 38, const int &b = 176);
    void HighlightKSeams(const std::vector<Seam> &seam, const int &r = 176, const int &g = 38, const int &b = 176);

    /** Display */
    void Export(const ImageType &type, const std::string &path);
    void Reset();
};
}  // namespace SeamCarving

#endif /** CarvableImage_h */
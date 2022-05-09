#include "pch.h"
#include <iostream>

namespace SeamCarving {
  class CarvableImage {
    private:
      /** Data members */
      Image original, res_img, trans_img;
      int rows_, cols_;

      /** Helper Funcs*/
      Seam __FindOptimalSeam(const Image &img, const Dir &dir);
      void __HighlightSeam(Image &img, const Seam &seam, const cv::Vec3b &color);
      Image __RemoveSeam(const Seam &seam, const Image &img);

    public:
      /** Constructors */
      CarvableImage(const string &path);
      
      /** Getters */
      inline int num_rows() const { return this->rows_; };
      inline int num_cols() const { return this->cols_; };

      /** Seams */
      void SeamCarve(const cv::Size &size);
      Seam FindOptimalSeam(const Dir &dir = VERT);
      void RemoveSeam(const Dir &dir);

      /** Highlighting */
      Image HighlightSeam(const Seam &seam, const cv::Vec3b &color);
      Image HighlightKSeams(const vector<Seam> &seams, const cv::Vec3b &color);
  };
}
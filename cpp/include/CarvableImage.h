#include <algorithm>
#include <vector>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "constants.h"
#include "energy.h"

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
      CarvableImage(const std::string &path);
      
      /** Getters */
      inline const Image& GetOriginal() const { return this->original; };
      inline const Image& GetResult() const { return this->res_img; };
      inline const Image& GetTranspose() const { return this->trans_img; };
      inline int num_rows() const { return this->rows_; };
      inline int num_cols() const { return this->cols_; };

      /** Seams */
      void SeamCarve(const int &width, const int &height);
      Seam FindOptimalSeam(const Dir &dir = VERT);
      void RemoveSeam(const Dir &dir);

      /** Highlighting */
      Image HighlightSeam(const Seam &seam, const cv::Vec3b &color);
      Image HighlightKSeams(const std::vector<Seam> &seams, const cv::Vec3b &color);

      /** Display */
      void Export(const ImageType &type, const std::string &path);
  };
}
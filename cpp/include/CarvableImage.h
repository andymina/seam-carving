/**
 * Andy Mina
 * 
 * Defines the class to carry out seam carving operations: CarvableImage. Simplifies the
 * user-facing logic to carve seams out of an image or insert them.
*/
#ifndef CarvableImage_h
#define CarvableImage_h

#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "constants.h"
#include "energy.h"

namespace SeamCarving {
  /**
   * Class that carries out seam carving operations.
  */
  class CarvableImage {
    private:
      /** Data members */
      Image original, res_img, trans_img;
      int rows_, cols_;

      /** Helper Funcs*/
      Seam __FindOptimalSeam(const Image &img, const Dir &dir);
      std::vector<Seam> __FindKOptimalSeams(const int &k, const Dir &dir, Image &img);
      void __HighlightSeam(Image &img, const Seam &seam, const cv::Vec3b &color);
      Image __RemoveSeam(const Seam &seam, const Image &img);
      Image __InsertSeam(const Seam &seam, const Image &img);

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
}

#endif /** CarvableImage_h */
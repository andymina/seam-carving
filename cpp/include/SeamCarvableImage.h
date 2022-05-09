#include "pch.h"

namespace SeamCarving {
  class CarvableImage {
    private:
      /** Helper Funcs*/
      Seam __FindOptimalSeam(const Image &img, const Dir &dir);
      void __RemoveSeam(const Seam &seam, const Image &img);


    public:

      /** Seams */
      void SeamCarve(const cv::Size &size);
      Seam FindOptimalSeam(const Dir &dir = VERT);
      void RemoveSeam(const Dir &dir);

      /** Highlighting */
      void HighlightSeam(const Seam &seam, const int &gray);
      void HighlightKSeams(const vector<Seam> &seams, const int &gray);
      void HighlightSeam(const Seam &seam, const vector<int> &color);
      void HighlightKSeams(const vector<Seam> &seams, const vector<int> &color);
  };
}
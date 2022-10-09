/**
 * Andy Mina
 * 
 * Helper file that define constants to be used throughout the project.
*/
#ifndef constants_h
#define constants_h

#include <opencv2/core.hpp>

namespace SeamCarving {
  /** Shorthand for defining an image. */
  typedef cv::Mat Image;

  /** A Enum to help better describe what direction to use in functions and seams. */
  enum Dir { VERT, HORZ };

  /** Helper representation of an (x,y) coord in an image in terms of (row, col). */
  struct Coord {
    int row, col;
    Coord(const int &row_ = 0, const int &col_ = 0):
      row{row_}, col{col_} {}
  };
  
  /**
   * Helper class to describe a seam.
   * 
   * @param dir the direction of the seam. must be one of [Dir.VERT, Dir.HORZ].
   * @param data a list representation of pixels in the seam specified by Coord.
  */
  struct Seam {
    Dir dir;
    std::vector<Coord> data;
    Seam(const Dir &dir_, const std::vector<Coord> &data_): 
      dir{dir_}, data{data_} {}
  };

  /** Helper enum to define the 6 types of images within a CarvableImage. */
  enum ImageType { ORIGINAL, RESULT, TRANSPOSE, ENERGY, VERT_MAP, HORZ_MAP };
}

#endif /* constants_h */
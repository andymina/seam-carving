/**
 * Andy Mina
 * 
 * Helper file that define constants to be used throughout the project
*/
#ifndef constants_h
#define constants_h

#include <opencv2/core.hpp>

namespace SeamCarving {
  /** Shorthand for defining an image. */
  typedef cv::Mat Image;

  /** A Enum to help better describe what direction to use in functions and seams. */
  enum Dir { VERT, HORZ };

  struct Coord {
    int row, col;

    Coord(const int &row_ = 0, const int &col_ = 0):
      row{row_}, col{col_} {}
  };
  
  /**
   * Helper class to describe a seam.
   * 
   * @param dir the direction of the seam. must be one of [Dir.VERT, Dir.HORZ].
   * @param data a list representation of the seam. If the seam is vertical, then the element at a
   * given index represents the column from which the pixel was removed in the row at that index.
   * For example, a vertical seam would be:
   * 
   *    arr: [0, 3, 1, 2]
   *    idx:  0  1  2  3
   * 
   * Pixels removed would be (row 0, column 0), (row 1, column 3), (row 2, column 1), and 
   * (row 3, column 2). The converse is true for horizontal seams. The index represents the column 
   * and the value represents the row.
  */
  struct Seam {
    Dir dir;
    std::vector<Coord> data;

    Seam(const Dir &dir_, const std::vector<Coord> &data_): 
      dir{dir_}, data{data_} {}
  };

  /** Helper enum to define the 5 types of images within a CarvableImage. */
  enum ImageType { ORIGINAL, RESULT, TRANSPOSE, ENERGY, VERT_MAP, HORZ_MAP };
}

#endif /* constants_h */
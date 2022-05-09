#include "CarvableImage.h"
#include "energy.h"
#include <algorithm>

namespace SeamCarving {
  /**
   * Constructor for the CarvableImage.
   * 
   * @param path file path to the Image to be carved
  */
  CarvableImage::CarvableImage(const string &path) {
    this->original = cv::imread(path);
    if (this->original.empty()) {
      std::cout << "Failed to read image: " << path << "\n";
      abort();
    }

    this->res_img = this->original.clone();
    this->trans_img = this->res_img.clone();
    cv::transpose(this->res_img, this->trans_img);
  }

  /**
   * Carves the current image (this->res_img) to the specified size. The size can be larger or smaller
   * than the current res_img in either dimension. The result is stored in this->res_img.
   * 
   * @param size the size of the desired image. (width, height)
   */
  void CarvableImage::SeamCarve(const cv::Size &size) {
    // remove seams if positive, insert seams if negative
    int row_diff = size.height - this->num_rows();
    int col_diff = size.width - this->num_cols();

    /** Decision tree. See samples/decision_tree.JPG */
    if (row_diff < 0) {
      if (col_diff < 0) {         // (-, -)

      } else if (col_diff == 0) { // (-, 0)
        while (row_diff != 0) {
          this->RemoveSeam(HORZ);
          row_diff++;
        }
      } else if (col_diff > 0) {  // (-, +)

      }
    } else if (row_diff == 0) {
      if (col_diff < 0) {         // (0, -)
        while (col_diff != 0) {
          this->RemoveSeam(VERT);
          col_diff++;
        }
      } else if (col_diff == 0) { // (0, 0)
        // no-op
      } else if (col_diff > 0) {  // (0, +)

      }
    } else if (row_diff > 0) {
      if (col_diff < 0) {         // (+, -)

      } else if (col_diff == 0) { // (+, 0)

      } else if (col_diff > 0) {  // (+, +)

      }
    }
  }

  /**
   * Returns the optimal seam to remove in the specified direction.
   * 
   * @param dir the direction of the seam to be find. Defaults to Dir.VERT.
   * 
   * @returns the seam
  */
  Seam CarvableImage::FindOptimalSeam(const Dir &dir) {
    return this->__FindOptimalSeam((dir == VERT) ? this->res_img : this->trans_img, dir);
  }

  /**
   * Removes a seam from the image in the specified direction. The result is stored within this->res_img.
   * 
   * @param dir the direction of the seam to be removed. Removing a VERT seam will reduce the number
   * of cols. Conversely, removing a HORZ seam will reduce the number of rows.
  */
  void CarvableImage::RemoveSeam(const Dir &dir) {
    Image res = this->__RemoveSeam(
      this->FindOptimalSeam(dir),
      (dir == VERT) ? this->res_img : this->trans_img
    );

    if (dir == VERT) {
      this->res_img = res;
      cv::transpose(this->res_img, this->trans_img);
      this->cols_--;
    } else if (dir == HORZ) {
      this->trans_img = res;
      cv::transpose(this->trans_img, this->res_img);
      this->rows_--;
    }
  }

  /** 
   * Highlights the given seam on this->res_img in the specified color. Color must be BGR format!
   * 
   * @param seam the seam to be highlighted
   * @param color the color to be used when highlighting. must be BGR!!
   * 
   * @returns a new Image with the seam highlighted
   */
  Image CarvableImage::HighlightSeam(const Seam &seam, const cv::Vec3b &color) {
    Image res = this->res_img.clone();
    this->__HighlightSeam(res, seam, color);
    return res;
  }

  /**
   * Highlights the k seams supplied in the given color. Color must be BGR format!
   * 
   * @param seams a vector of Seams to be highlighted
   * @param color the color to be used when highlighting. must be BGR!!
   * 
   * @returns a new Image with the k seams highlighted 
  */
  Image CarvableImage::HighlightKSeams(const vector<Seam> &seams, const cv::Vec3b &color) {
    Image res = this->res_img.clone();
    for (const Seam &seam: seams)
      this->__HighlightSeam(res, seam, color);
    return res;
  }

  /**
   * Returns the optimal vertical seam in the given image.
   * 
   * @param img the Image to find the optimal seam in
   * @param dir the direction to find the optimal seam in 
   * 
   * @returns the optimal seam in the given direction
   */
  Seam CarvableImage::__FindOptimalSeam(const Image &img, const Dir &dir) {
    // find starting point
    Image energy_map = Energy::ComputeEnergyMap(Energy::ComputeEnergy(img, "sobel"));
    int idx;
    cv::minMaxIdx(energy_map.row(0), nullptr, nullptr, &idx, nullptr);

    Seam seam = Seam(dir, {});
    int rows = energy_map.rows;
    int cols = energy_map.cols;

    for (int row = 0; row < rows; row++) {
      seam.data.push_back(idx);

      if (row != rows - 1) {
        // find the direction of min and adjust path
        const int &center = energy_map.at<int>(row + 1, idx);
        const int &left = (idx - 1 >= 0) ? energy_map.at<int>(row + 1, idx + 1) : INT_MAX;
        const int &right = (idx + 1 < cols) ? energy_map.at<int>(row + 1, idx + 1) : INT_MAX;
        int min_energy = std::min({center, left, right});

        if (min_energy == left)
          idx--;
        else if (min_energy == right)
          idx++;
      }
    }

    return seam;
  }

  /**
   * Highlights the seam in the given color on the given image. Modifies the supplied image directly.
   * 
   * @param img the Image to highlight the seam on
   * @param seam the seam to be highlighted
   * @param color the color to highlight the seam in. must be BGR format!!
  */
  void CarvableImage::__HighlightSeam(Image &img, const Seam &seam, const cv::Vec3b &color) {
    if (seam.dir == VERT)
      for (int idx = 0; idx < seam.data.size(); idx++)
        img.at<cv::Vec3b>(idx, seam.data[idx]) = color;
    else if (seam.dir == HORZ)
      for (int idx = 0; idx < seam.data.size(); idx++)
        img.at<cv::Vec3b>(seam.data[idx], idx) = color;
  }

  /**
   * Removes the specified seam from the image.
   * 
   * @param seam the seam to be removed
   * @param img the image to remove the seam from
   * 
   * @returns a new Image with the seam removed
  */
  Image CarvableImage::__RemoveSeam(const Seam &seam, const Image &img) {
    /** TODO: complete this and test existing functionality in standalone build*/
  }
}
#include "CarvableImage.h"

namespace SeamCarving {
  /**
   * Constructor for the CarvableImage.
   * 
   * @param path file path to the Image to be carved
  */
  CarvableImage::CarvableImage(const std::string &path) {
    this->original = cv::imread(path);
    if (this->original.empty()) {
      std::cout << "Failed to read image: " << path << "\n";
      abort();
    }

    this->res_img = this->original.clone();
    cv::transpose(this->res_img, this->trans_img);
    this->rows_ = this->res_img.rows;
    this->cols_ = this->res_img.cols;
  }

  /**
   * Carves the current image (this->res_img) to the specified size. The size can be larger or smaller
   * than the current res_img in either dimension. The result is stored in this->res_img.
   * 
   * @param size the size of the desired image. (width, height)
   */
  void CarvableImage::SeamCarve(const int &width, const int &height) {
    // remove seams if positive, insert seams if negative
    int row_diff = height - this->num_rows();
    int col_diff = width - this->num_cols();

    /** Decision tree. See samples/decision_tree.JPG */
    if (row_diff < 0) {
      if (col_diff < 0) {         // (-, -) done
        int total = row_diff + col_diff;
        while (total != 0) {
          if (col_diff != 0) {
            this->RemoveSeam(VERT);
            col_diff++;
            total++;
          }

          if (row_diff != 0) {
            this->RemoveSeam(HORZ);
            row_diff++;
            total++;
          }
        }
      } else if (col_diff == 0) { // (-, 0) done
        while (row_diff != 0) {
          this->RemoveSeam(HORZ);
          row_diff++;
        }
      } else if (col_diff > 0) {  // (-, +)

      }
    } else if (row_diff == 0) {
      if (col_diff < 0) {         // (0, -) done
        while (col_diff != 0) {
          this->RemoveSeam(VERT);
          col_diff++;
        }
      } else if (col_diff == 0) { // (0, 0) done
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

  std::vector<Seam> CarvableImage::FindKOptimalSeams(const int &k, const Dir &dir) {
    Image img = (dir == VERT) ? this->res_img.clone() : this->trans_img.clone();
    std::vector<Seam> res = this->__FindKOptimalSeams(k, VERT, img);
    
    if (dir == HORZ)
      for (Seam &seam: res)
        seam.dir = HORZ;
    
    return res;
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

  void CarvableImage::InsertSeam(const Dir &dir) {
    Seam seam = this->FindOptimalSeam(dir);
    Image res = this->__InsertSeam(seam, (dir == VERT) ? this->res_img : this->trans_img);
    
    if (dir == VERT) {
      this->res_img = res;
      cv::transpose(this->res_img, this->trans_img);
      this->cols_++;
    } else if (dir == HORZ) {
      this->trans_img = res;
      cv::transpose(this->trans_img, this->res_img);
      this->rows_++;
    }
  }

  void CarvableImage::InsertKSeams(const int &k, const Dir &dir) {
    std::vector<Seam> seams = this->FindKOptimalSeams(k, dir);
    Image res = (dir == VERT) ? this->res_img.clone() : this->trans_img.clone();

    for (const Seam &s: seams)
      res = this->__InsertSeam(s, res);

    if (dir == VERT) {
      this->res_img = res;
      cv::transpose(this->res_img, this->trans_img);
      this->cols_ += k;
    } else if (dir == HORZ) {
      this->trans_img = res;
      cv::transpose(this->trans_img, this->res_img);
      this->rows_ += k;
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
  void CarvableImage::HighlightSeam(const Seam &seam, const int &r, const int &g, const int &b) {
    this->__HighlightSeam(this->res_img, seam, cv::Vec3b(r, g, b));
  }

  /**
   * Highlights the k seams supplied in the given color. Color must be BGR format!
   * 
   * @param seams a vector of Seams to be highlighted
   * @param color the color to be used when highlighting. must be BGR!!
   * 
   * @returns a new Image with the k seams highlighted 
  */
  void CarvableImage::HighlightKSeams(const std::vector<Seam> &seams, const int &r, const int &g, const int &b) {
    Image &res = (seams[0].dir == VERT) ? this->res_img : this->trans_img;
    for (const Seam &seam: seams)
      this->__HighlightSeam(res, seam, cv::Vec3b(r, g, b));

    if (seams[0].dir == HORZ)
      cv::transpose(this->trans_img, this->res_img);
  }

  /**
   * Displays the target image which is specified 
   * 
   * @param target 
  */
  void CarvableImage::Export(const ImageType &type, const std::string &path) {    
    switch (type) {
      case ORIGINAL: {
        cv::imwrite(path, this->original);
        break;
      }
      
      case RESULT: {
        cv::imwrite(path, this->res_img);
        break;
      }

      case TRANSPOSE: {
        cv::imwrite(path, this->trans_img);
        break;
      }
      
      case ENERGY: {
        cv::imwrite(path, Energy::ComputeEnergy(this->res_img, "sobel"));
        break;
      }

      case VERT_MAP: {
        Image map = Energy::ComputeEnergyMap(Energy::ComputeEnergy(this->res_img, "sobel"));
        cv::normalize(map, map, 0, 255, cv::NORM_MINMAX); // normalize to correct range
        cv::imwrite(path, map);
        break;
      }

      case HORZ_MAP: {
        Image map = Energy::ComputeEnergyMap(Energy::ComputeEnergy(this->trans_img, "sobel"));
        cv::normalize(map, map, 0, 255, cv::NORM_MINMAX); // normalize to correct range
        cv::transpose(map, map);
        cv::imwrite(path, map);
        break;
      }
    }
  }
  
  /**
   * 
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
    int idx = 0, minVal = energy_map.row(0).at<int>(idx);
    for (int i = 0; i < energy_map.row(0).cols; i++)
      if (energy_map.row(0).at<int>(i) < minVal) {
        idx = i;
        minVal = energy_map.row(0).at<int>(idx);
      }

    Seam seam = Seam(dir, {});
    int rows = energy_map.rows, cols = energy_map.cols;

    if (dir == VERT) {
      for (int row = 0; row < rows; row++) {
        seam.data.push_back({row, idx});

        if (row != rows - 1) {
          // find the direction of min and adjust path
          const int &center = energy_map.at<int>(row + 1, idx);
          const int &left = (idx - 1 < 0) ? std::numeric_limits<int>::max() : energy_map.at<int>(row + 1, idx - 1);
          const int &right = (idx + 1 >= cols) ? std::numeric_limits<int>::max() : energy_map.at<int>(row + 1, idx + 1);
          int min_energy = std::min({center, left, right});

          if (min_energy == left) idx--;
          else if (min_energy == right) idx++;
        }
      }
    } else if (dir == HORZ) {
      for (int row = 0; row < rows; row++) {
        seam.data.push_back({idx, row});

        if (row != rows - 1) {
          // find the direction of min and adjust path
          const int &center = energy_map.at<int>(row + 1, idx);
          const int &left = (idx - 1 < 0) ? std::numeric_limits<int>::max() : energy_map.at<int>(row + 1, idx - 1);
          const int &right = (idx + 1 >= cols) ? std::numeric_limits<int>::max() : energy_map.at<int>(row + 1, idx + 1);
          int min_energy = std::min({center, left, right});

          if (min_energy == left) idx--;
          else if (min_energy == right) idx++;
        }
      }
    }

    return seam;
  }

  std::vector<Seam> CarvableImage::__FindKOptimalSeams(const int &k, const Dir &dir, Image &img) {
    std::vector<Seam> res;
    int count = k;
    while (count != 0) {
      Seam seam = this->__FindOptimalSeam(img, dir);
      res.push_back(seam);
      img = this->__RemoveSeam(seam, img);
      count--;
    }
    return res;
  }

  /**
   * Highlights the seam in the given color on the given image. Modifies the supplied image directly.
   * 
   * @param img the Image to highlight the seam on
   * @param seam the seam to be highlighted
   * @param color the color to highlight the seam in. must be BGR format!!
  */
  void CarvableImage::__HighlightSeam(Image &img, const Seam &seam, const cv::Vec3b &color) {
    for (int idx = 0; idx < seam.data.size(); idx++)
      img.at<cv::Vec3b>(seam.data[idx].row, seam.data[idx].col) = color;
  }

  /**
   * Removes the specified seam from the image vertically.
   * 
   * @param seam the seam to be removed
   * @param img the image to remove the seam from
   * 
   * @returns a new Image with the seam removed
  */
  Image CarvableImage::__RemoveSeam(const Seam &seam, const Image &img) {
    /** TODO: complete this and test existing functionality in standalone build */
    Image res = Image(img.rows, img.cols - 1, img.type());

    for (int idx = 0; idx < seam.data.size(); idx++) {
      const cv::Mat &current_row = img.row(seam.data[idx].row);

      if (seam.data[idx].col == 0) {
        // exclude first val
        current_row.colRange(1, img.cols).copyTo(res.row(seam.data[idx].row));
      } else if (seam.data[idx].col == img.cols - 1) {
        // exclude last val
        current_row.colRange(0, img.cols - 1).copyTo(res.row(seam.data[idx].row));
      } else {
        // merge two halves
        cv::hconcat(
          current_row.colRange(0, seam.data[idx].col),
          current_row.colRange(seam.data[idx].col + 1, img.cols),
          res.row(seam.data[idx].row)
        );
      }
    }
    
    return res;  
  }

  /**
   * TODO:
   * 
   * get the current color and make a 1x1 matrix
   * get the rest of the OG matrix
   * concat all of the mats
   * average down the seam
  */
  Image CarvableImage::__InsertSeam(const Seam &seam, const Image &img) {
    Image res = cv::Mat(img.rows, img.cols + 1, img.type());

    for (int idx = 0; idx < seam.data.size(); idx++) {
      const cv::Mat &current_row = img.row(seam.data[idx].row);
      std::vector<cv::Mat> mats;

      cv::Mat col_insert = cv::Mat(1, 1, img.type());
      col_insert.at<cv::Vec3b>(0, 0) = img.at<cv::Vec3b>(seam.data[idx].row, seam.data[idx].col);

      if (seam.data[idx].col == 0) {
        mats.push_back(col_insert);
        mats.push_back(current_row);
      } else if (seam.data[idx].col == img.cols - 1) {
        mats.push_back(current_row);
        mats.push_back(col_insert);
      } else {
        mats.push_back(current_row.colRange(0, seam.data[idx].col));
        mats.push_back(col_insert);
        mats.push_back(current_row.colRange(seam.data[idx].col, img.cols));
      }
      cv::hconcat(mats, res.row(seam.data[idx].row));

      for (const Coord &c: seam.data) {
        // average only if i have left/right neighbors

        if (c.col - 1 >= 0 && c.col + 1 < res.cols) {
          const cv::Vec3b &left = res.at<cv::Vec3b>(c.row, c.col - 1);
          const cv::Vec3b &right = res.at<cv::Vec3b>(c.row, c.col + 1);
          cv::Vec3b color = cv::Vec3b(
            (left[0] + right[0]) / 2,
            (left[1] + right[1]) / 2,
            (left[2] + right[2]) / 2
          );
        }
      }
    }

    return res;
  }

  void CarvableImage::Reset() {
    this->res_img = this->original.clone();
    cv::transpose(this->res_img, this->trans_img);
  }
}
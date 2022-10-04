/**
 * Andy Mina
 * 
 * Implementation of the functions defined in CarvableImage.h
*/
#include "CarvableImage.h"

namespace SeamCarving {
  /**
   * Constructor for the CarvableImage.
   * 
   * this->original - stores the original image for comparison and if needed to reset
   * this->res_img - the resulting image after any seam operations
   * this->trans_img - the transposed image of this->res_img used for internal calculations
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
  void CarvableImage::SeamCarve(const int &target_rows, const int &target_cols) {
    // remove seams if positive, insert seams if negative
    int row_diff = target_rows - this->num_rows();
    int col_diff = target_cols - this->num_cols();

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
      } else if (col_diff > 0) {  // (-, +) done
        this->InsertKSeams(col_diff, VERT);

        while(row_diff != 0) {
          this->RemoveSeam(HORZ);
          row_diff++;
        }
      }
    } else if (row_diff == 0) {
      if (col_diff < 0) {         // (0, -) done
        while (col_diff != 0) {
          this->RemoveSeam(VERT);
          col_diff++;
        }
      } else if (col_diff == 0) { // (0, 0) done
        // no-op
      } else if (col_diff > 0) {  // (0, +) done
        this->InsertKSeams(col_diff, VERT);
      }
    } else if (row_diff > 0) {
      if (col_diff < 0) {         // (+, -) done
        this->InsertKSeams(row_diff, HORZ);

        while (col_diff != 0) {
          this->RemoveSeam(VERT);
          col_diff++;
        }
      } else if (col_diff == 0) { // (+, 0) done
        this->InsertKSeams(row_diff, HORZ);
      } else if (col_diff > 0) {  // (+, +) done
        int total = row_diff + col_diff;

        while (total != 0) {
          if (col_diff != 0) {
            this->InsertSeam(VERT);
            col_diff--;
            total--;
          }

          if (row_diff != 0) {
            this->InsertSeam(HORZ);
            row_diff--;
            total--;
          }
        }
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
    return (dir == VERT) ?
      this->__FindOptimalVerticalSeam(this->res_img) :
      this->__FindOptimalHorizontalSeam(this->res_img);
  }

  /** 
   * Finds the first K optimal seams in the specified direction in image.
   * 
   * @param k the number of seams to find
   * @param dir the direction of the seam to be found.
   * @returns a vector of the seams.
  */
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
    if (dir == VERT) {
      const Seam &seam = this->__FindOptimalVerticalSeam(this->res_img);
      this->res_img = this->__RemoveVerticalSeam(seam, this->res_img);
      this->cols_--;
    } else if (dir == HORZ) {
      const Seam &seam = this->__FindOptimalHorizontalSeam(this->res_img);
      this->res_img = this->__RemoveHorizontalSeam(seam, this->res_img);
      this->rows_--;
    }

    cv::transpose(this->res_img, this->trans_img);
  }

  /**
   * Inserts the optimal seam into the image in the specified direction. The result is stored within
   * this->res_img.
   * 
   * @param dir the direction of the seam to inserted.
  */
  void CarvableImage::InsertSeam(const Dir &dir) {
    Seam seam = this->FindOptimalSeam(dir);
    this->res_img = (dir == VERT) ?
      this->__InsertVerticalSeam(seam, this->res_img) :
      this->__InsertHorizontalSeam(seam, this->res_img);

    if (dir == VERT) this->cols_++;
    else this->rows_++;

    cv::transpose(this->res_img, this->trans_img);
  }

  /**
   * Inserts the first k seams into the image in the specified direction. Starts by finding the
   * first k seams for removal and then inserts them in order of arrival.
   * 
   * @param k the number of seams to be inserted
   * @param dir the direction of seams to be inserted
  */
  void CarvableImage::InsertKSeams(const int &k, const Dir &dir) {
    std::vector<Seam> seams = this->FindKOptimalSeams(k, dir);
    Image res = (dir == VERT) ? this->res_img.clone() : this->trans_img.clone();

    if (dir == HORZ)
      for (Seam &s: seams)
        s.dir = VERT;

    for (const Seam &s: seams)
      res = this->__InsertVerticalSeam(s, res);

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
   * Saves the target image which is specified by the enums listed in sc::ImageType to the
   * specified path.
   * 
   * @param type the type of image to be saved as defined by sc::ImageType enums
   * @param path the path to save the image to
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
        Image map = Energy::ComputeVerticalEnergyMap(Energy::ComputeEnergy(this->res_img, "sobel"));
        cv::normalize(map, map, 0, 255, cv::NORM_MINMAX); // normalize to correct range
        cv::imwrite(path, map);
        break;
      }

      case HORZ_MAP: {
        Image map = Energy::ComputeHorizontalEnergyMap(Energy::ComputeEnergy(this->trans_img, "sobel"));
        cv::normalize(map, map, 0, 255, cv::NORM_MINMAX); // normalize to correct range
        cv::transpose(map, map);
        cv::imwrite(path, map);
        break;
      }
    }
  }
  
  /**
   * Resets the Carver so that this->res_img is the original image read.
  */
  void CarvableImage::Reset() {
    this->res_img = this->original.clone();
    cv::transpose(this->res_img, this->trans_img);
    this->rows_ = this->res_img.rows;
    this->cols_ = this->res_img.cols;
  }
  
  Seam CarvableImage::__FindOptimalVerticalSeam(const Image &img) {
    // find starting point
    Image energy_map = Energy::ComputeVerticalEnergyMap(Energy::ComputeEnergy(img, "sobel"));
    int idx = 0, minVal = energy_map.row(0).at<int>(idx);
    for (int i = 0; i < energy_map.row(0).cols; i++)
      if (energy_map.row(0).at<int>(i) < minVal) {
        idx = i;
        minVal = energy_map.row(0).at<int>(idx);
      }

    Seam seam = Seam(VERT, {});
    int rows = energy_map.rows, cols = energy_map.cols;

    for (int row = 0; row < rows; row++) {
      seam.data.push_back({row, idx});

      if (row != rows - 1) {
        // find the direction of min and adjust path
        const int &center = energy_map.at<int>(row + 1, idx);
        const int &left = (idx - 1 < 0) ? std::numeric_limits<int>::max() : energy_map.at<int>(row + 1, idx - 1);
        const int &right = (idx + 1 >= cols) ? std::numeric_limits<int>::max() : energy_map.at<int>(row + 1, idx + 1);

        // find the min pixel and adjust weight
        int min_energy = std::min({center, left, right});
        if (min_energy == left) idx--;
        else if (min_energy == right) idx++;
      }
    }
   
    return seam;
  }

  Seam CarvableImage::__FindOptimalHorizontalSeam(const Image &img) {
    // find starting point
    Image energy_map = Energy::ComputeHorizontalEnergyMap(Energy::ComputeEnergy(img, "sobel"));
    int idx = 0, minVal = energy_map.col(0).at<int>(idx);
    for (int i = 0; i < energy_map.col(0).rows; i++)
      if (energy_map.col(0).at<int>(i) < minVal) {
        idx = i;
        minVal = energy_map.col(0).at<int>(idx);
      }

    Seam seam = Seam(HORZ, {});
    int rows = energy_map.rows, cols = energy_map.cols;

    for (int row = 0; row < rows; row++) {
      seam.data.push_back({idx, row});

      if (row != rows - 1) {
        // find the direction of min and adjust path
        const int &center = energy_map.at<int>(row + 1, idx);
        const int &left = (idx - 1 < 0) ? std::numeric_limits<int>::max() : energy_map.at<int>(row + 1, idx - 1);
        const int &right = (idx + 1 >= cols) ? std::numeric_limits<int>::max() : energy_map.at<int>(row + 1, idx + 1);

        // find the min pixel and adjust weight
        int min_energy = std::min({center, left, right});
        if (min_energy == left) idx--;
        else if (min_energy == right) idx++;
      }
    }
  }

  /**
   * Internal method to find the first k seams for removal.
   * 
   * @param k the number of seams to find
   * @param dir the direction of the seams to be found
   * @param img the image to find the seams in; modified to find k seams
  */
  std::vector<Seam> CarvableImage::__FindKOptimalSeams(const int &k, const Dir &dir, Image &img) {
    std::vector<Seam> res;
    int count = k;

    if (dir == VERT)
      while (count != 0) {
        Seam seam = this->__FindOptimalVerticalSeam(img);
        res.push_back(seam);
        img = this->__RemoveVerticalSeam(seam, img);
        count--;
      }
    else 
      while (count != 0) {
        Seam seam = this->__FindOptimalHorizontalSeam(img);
        res.push_back(seam);
        img = this->__RemoveHorizontalSeam(seam, img);
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

  Seam CarvableImage::__FindOptimalVerticalSeam(const Image &img) {
    // find starting point
    Image energy_map = Energy::ComputeVerticalEnergyMap(Energy::ComputeEnergy(img, "sobel"));
    int idx = 0, minVal = energy_map.row(0).at<int>(idx);
    for (int i = 0; i < energy_map.row(0).cols; i++)
      if (energy_map.row(0).at<int>(i) < minVal) {
        idx = i;
        minVal = energy_map.row(0).at<int>(idx);
      }

    Seam seam = Seam(VERT, {});
    int rows = energy_map.rows, cols = energy_map.cols;

    for (int row = 0; row < rows; row++) {
      seam.data.push_back({row, idx});

      if (row != rows - 1) {
        // find the direction of min and adjust path
        const int &center = energy_map.at<int>(row + 1, idx);
        const int &left = (idx - 1 < 0) ? std::numeric_limits<int>::max() : energy_map.at<int>(row + 1, idx - 1);
        const int &right = (idx + 1 >= cols) ? std::numeric_limits<int>::max() : energy_map.at<int>(row + 1, idx + 1);

        // find the min pixel and adjust weight
        int min_energy = std::min({center, left, right});
        if (min_energy == left) idx--;
        else if (min_energy == right) idx++;
      }
    }
   
    return seam;
  }

  Seam CarvableImage::__FindOptimalHorizontalSeam(const Image &img) {
    // find starting point
    Image energy_map = Energy::ComputeHorizontalEnergyMap(Energy::ComputeEnergy(img, "sobel"));
    int idx = 0, minVal = energy_map.col(0).at<int>(idx);
    for (int i = 0; i < energy_map.col(0).rows; i++)
      if (energy_map.col(0).at<int>(i) < minVal) {
        idx = i;
        minVal = energy_map.col(0).at<int>(idx);
      }

    Seam seam = Seam(HORZ, {});
    int rows = energy_map.rows, cols = energy_map.cols;

    for (int row = 0; row < rows; row++) {
      seam.data.push_back({idx, row});

      if (row != rows - 1) {
        // find the direction of min and adjust path
        const int &center = energy_map.at<int>(row + 1, idx);
        const int &left = (idx - 1 < 0) ? std::numeric_limits<int>::max() : energy_map.at<int>(row + 1, idx - 1);
        const int &right = (idx + 1 >= cols) ? std::numeric_limits<int>::max() : energy_map.at<int>(row + 1, idx + 1);

        // find the min pixel and adjust weight
        int min_energy = std::min({center, left, right});
        if (min_energy == left) idx--;
        else if (min_energy == right) idx++;
      }
    }
  }

  Image CarvableImage::__RemoveVerticalSeam(const Seam &seam, const Image &img) {
    Image res = Image(img.rows, img.cols - 1, img.type());

    for (int idx = 0; idx < seam.data.size(); idx++) {
      const cv::Mat &current_row = img.row(idx);

      if (seam.data[idx].col == 0) {
        // exclude first val
        current_row.colRange(1, img.cols).copyTo(res.row(idx));
      } else if (seam.data[idx].col == img.cols - 1) {
        // exclude last val
        current_row.colRange(0, img.cols - 1).copyTo(res.row(idx));
      } else {
        // merge two halves
        cv::hconcat(
          current_row.colRange(0, seam.data[idx].col),
          current_row.colRange(seam.data[idx].col + 1, img.cols),
          res.row(idx)
        );
      }
    }
    
    return res;  
  }

  Image CarvableImage::__RemoveHorizontalSeam(const Seam &seam, const Image &img) {
    Image res = Image(img.rows - 1, img.cols, img.type());

    for (int idx = 0; idx < seam.data.size(); idx++) {
      const cv::Mat &current_row = img.row(seam.data[idx].col);

      if (seam.data[idx].row == 0) {
        // exclude first val
        current_row.colRange(1, img.cols).copyTo(res.row(seam.data[idx].col));
      } else if (seam.data[idx].row == img.cols - 1) {
        // exclude last val
        current_row.colRange(0, img.cols - 1).copyTo(res.row(seam.data[idx].col));
      } else {
        // merge two halves
        cv::hconcat(
          current_row.colRange(0, seam.data[idx].row),
          current_row.colRange(seam.data[idx].row + 1, img.cols),
          res.row(seam.data[idx].col)
        );
      }
    }

    return res;
  }

  Image CarvableImage::__InsertVerticalSeam(const Seam &seam, const Image &img) {
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

  Image CarvableImage::__InsertHorizontalSeam(const Seam &seam, const Image &img) {
    Image res = cv::Mat(img.rows, img.cols + 1, img.type());

    for (int idx = 0; idx < seam.data.size(); idx++) {
      const cv::Mat &current_row = img.row(seam.data[idx].col);
      std::vector<cv::Mat> mats;

      cv::Mat col_insert = cv::Mat(1, 1, img.type());
      col_insert.at<cv::Vec3b>(0, 0) = img.at<cv::Vec3b>(seam.data[idx].col, seam.data[idx].row);

      if (seam.data[idx].row == 0) {
        mats.push_back(col_insert);
        mats.push_back(current_row);
      } else if (seam.data[idx].row == img.cols - 1) {
        mats.push_back(current_row);
        mats.push_back(col_insert);
      } else {
        mats.push_back(current_row.colRange(0, seam.data[idx].row));
        mats.push_back(col_insert);
        mats.push_back(current_row.colRange(seam.data[idx].row, img.cols));
      }
      cv::hconcat(mats, res.row(seam.data[idx].col));

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
  }
}
/**
 * Andy Mina
 */
#include "Carver.h"

namespace SeamCarving {
Seam Carver::FindOptimalVerticalSeam(const Image &img) {
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
            if (min_energy == left)
                idx--;
            else if (min_energy == right)
                idx++;
        }
    }

    return seam;
}

Seam Carver::FindOptimalHorizontalSeam(const Image &img) {
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
            if (min_energy == left)
                idx--;
            else if (min_energy == right)
                idx++;
        }
    }

    return seam;
}

void Carver::RemoveVerticalSeam(const Seam &seam, Image &img) {
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
                res.row(idx));
        }
    }

    img = std::move(res);
}

void Carver::RemoveHorizontalSeam(const Seam &seam, Image &img) {
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
                res.row(seam.data[idx].col));
        }
    }

    img = std::move(res);
}

void Carver::InsertVerticalSeam(const Seam &seam, Image &img) {
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

        for (const Coord &c : seam.data) {
            // average only if i have left/right neighbors

            if (c.col - 1 >= 0 && c.col + 1 < res.cols) {
                const cv::Vec3b &left = res.at<cv::Vec3b>(c.row, c.col - 1);
                const cv::Vec3b &right = res.at<cv::Vec3b>(c.row, c.col + 1);
                cv::Vec3b color = cv::Vec3b(
                    (left[0] + right[0]) / 2,
                    (left[1] + right[1]) / 2,
                    (left[2] + right[2]) / 2);
            }
        }
    }

    img = std::move(res);
}

void Carver::InsertHorizontalSeam(const Seam &seam, Image &img) {
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

        for (const Coord &c : seam.data) {
            // average only if i have left/right neighbors

            if (c.col - 1 >= 0 && c.col + 1 < res.cols) {
                const cv::Vec3b &left = res.at<cv::Vec3b>(c.row, c.col - 1);
                const cv::Vec3b &right = res.at<cv::Vec3b>(c.row, c.col + 1);
                cv::Vec3b color = cv::Vec3b(
                    (left[0] + right[0]) / 2,
                    (left[1] + right[1]) / 2,
                    (left[2] + right[2]) / 2);
            }
        }
    }

    img = std::move(res);
}

}  // namespace SeamCarving
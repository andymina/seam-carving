//
// Created by Andy Mina on 3/7/23.
//

// project
#include <seam_carving/carver.hpp>
#include <iostream>

namespace seam_carving {
    Seam Carver::FindVerticalSeam(cv::InputArray img) {
        // compute the energy map
        cv::Mat energy_map;
        energy::ComputeEnergy(img, energy_map);
        energy::ComputeVerticalMap(energy_map, energy_map);

        // find the starting val
        ushort col = 0, min_val = energy_map.at<ushort>(0, col);
        for (int i = 0; i < energy_map.cols; i++)
            if (energy_map.at<ushort>(0, i) < min_val) {
                col = i;
                min_val = energy_map.at<ushort>(0, col);
            }

        Seam seam = Seam(VERT);
        int rows = energy_map.rows, cols = energy_map.cols;

        // loop through each row
        for (int row = 0; row < rows; row++) {
            // add the current Coord to the seam
            seam.push(Coord(row, col));

            if (row != rows - 1) {
                // find the direction of min and adjust path
                const ushort& center = energy_map.at<ushort>(row + 1, col);
                const ushort& left = (col - 1 < 0) ?
                                     std::numeric_limits<ushort>::max() :
                                     energy_map.at<ushort>(row + 1, col - 1);
                const ushort& right = (col + 1 >= cols) ?
                                      std::numeric_limits<ushort>::max() :
                                      energy_map.at<ushort>(row + 1, col + 1);

                // find the min pixel and adjust weight
                if (left < center && left < right) col--;
                else if (right < center && right < left) col++;
            }
        }

        return seam;
    }

    Seam Carver::FindHorizontalSeam(cv::InputArray img) {
        // compute the energy map
        cv::Mat energy_map;
        energy::ComputeEnergy(img, energy_map);
        energy::ComputeHorizontalMap(energy_map, energy_map);

        // find the starting val
        ushort row = 0, min_val = energy_map.at<ushort>(row, 0);
        for (int i = 0; i < energy_map.rows; i++)
            if (energy_map.at<ushort>(i, 0) < min_val) {
                row = i;
                min_val = energy_map.at<ushort>(i, 0);
            }

        Seam seam = Seam(HORZ);
        int rows = energy_map.rows, cols = energy_map.cols;

        // loop through each col
        for (int col = 0; col < cols; col++) {
            // add the current Coord to the seam
            seam.push(Coord(row, col));

            if (col != cols - 1) {
                // find the direction of min and adjust path
                const ushort &center = energy_map.at<ushort>(row, col + 1);
                const ushort &above = (row - 1 < 0) ?
                                      std::numeric_limits<ushort>::max() :
                                      energy_map.at<ushort>(row - 1, col + 1);
                const ushort &below = (row + 1 >= rows) ?
                                      std::numeric_limits<ushort>::max() :
                                      energy_map.at<ushort>(row + 1, col + 1);

                // find the min pixel and adjust weight
                if (above < center && above < below) row--;
                else if (below < center && below < above) row++;
            }
        }

        return seam;
    }

    void Carver::RemoveVerticalSeam(const Seam &seam, cv::InputArray input, cv::OutputArray output) {
        // setup
        cv::Mat in_mat = input.getMat();
        cv::Mat res = cv::Mat(in_mat.rows, in_mat.cols - 1, in_mat.type());

        // loop through all rows
        for (int idx = 0; idx < res.rows; idx++) {
            // grab the current row and coord
            const cv::Mat &current_src_row = in_mat.row(idx);
            const Coord &current_coord = seam[idx];

            // check where to split the image since you cant have jagged matrix. colRange is exclusive
            if (current_coord.col() == 0) { // seam removes (row, 0)
                // copy (row, 1) to (row, in_mat.cols)
                current_src_row.colRange(1, in_mat.cols).copyTo(res.row(idx));
            } else if (current_coord.col() == in_mat.cols - 1) { // seam removes (row, in_mat.cols - 1)
                // copy (row, 0) to (row, in_mat.cols - 1)
                current_src_row.colRange(0, in_mat.cols - 1).copyTo(res.row(idx));
            } else { // seam removes (row, current_coord.col()
                // copy (row, 0) to (row, current_coord.col()
                const cv::Mat &left_partition = current_src_row.colRange(0, current_coord.col()).clone();
                // copy (row, current_coord.col() + 1) to (row, in_mat.cols)
                const cv::Mat &right_partition = current_src_row.colRange(current_coord.col() + 1, in_mat.cols).clone();
                // merge the two halves and store into row
                cv::hconcat(left_partition, right_partition, res.row(idx));
            }
        }

        res.copyTo(output);
    }

    void Carver::RemoveHorizontalSeam(const Seam &seam, cv::InputArray input, cv::OutputArray output) {
        // setup
        cv::Mat in_mat = input.getMat();
        cv::Mat res = cv::Mat(in_mat.rows - 1, in_mat.cols, in_mat.type());

        // loop through all cols
        for (int idx = 0; idx < res.cols; idx++) {
            // grab the current col and coord
            const cv::Mat &current_src_col = in_mat.col(idx);
            const Coord &current_coord = seam[idx];

            // check where to split the image since you cant have jagged matrix. rowRange is exclusive
            if (current_coord.row() == 0) { // seam removes (0, col)
                // copy (1, col) to (in_mat.rows, col)
                current_src_col.rowRange(1, in_mat.rows).copyTo(res.col(idx));
            } else if (current_coord.row() == in_mat.rows - 1) { // seam removes (in_mat.rows - 1, col)
                // copy (0, col) to (in_mat.rows - 1, col)
                current_src_col.rowRange(0, in_mat.rows - 1).copyTo(res.col(idx));
            } else { // seam removes (current_coord.row, col)
                // copy (0, col) to (current_coord.row, col)
                const cv::Mat &top_partition = current_src_col.rowRange(0, current_coord.row());
                // copy (current_coord.row() + 1, col) to (in_mat.rows, col)
                const cv::Mat &bottom_partition = current_src_col.rowRange(current_coord.row() + 1, in_mat.rows);
                // merge the two halves and store into row
                cv::vconcat(top_partition, bottom_partition, res.col(idx));
            }
        }

        res.copyTo(output);
    }

    void Carver::InsertVerticalSeam(const Seam &seam, cv::InputArray input, cv::OutputArray output) {
        cv::Mat in_mat = input.getMat();
        cv::Mat res = cv::Mat(in_mat.rows, in_mat.cols + 1, in_mat.type());

        // loop through all rows
        for (int idx = 0; idx < res.rows; idx++) {
            // grab the current row and coord
            const cv::Mat &current_src_row = in_mat.row(idx);
            const Coord &current_coord = seam[idx];
            std::vector<cv::Mat> mats;

            // create a 1x1 to insert with the pixel in the input
            cv::Mat col_insert = cv::Mat(1, 1, in_mat.type());
            cv::Mat left_partition, right_partition;

            // Insertion is always done such that an inserted pixel has neighbors and can be color corrected
            if (current_coord.col() == 0) {
                // insert the new pixel into index 1
                left_partition = current_src_row.colRange(0, 1).clone();
                right_partition = current_src_row.colRange(1, in_mat.cols).clone();
            } else if (current_coord.col() == in_mat.cols - 1) {
                // insert the new pixel into in_mat.cols - 2
                left_partition = current_src_row.colRange(0, in_mat.cols - 1).clone();
                right_partition = current_src_row.colRange(in_mat.cols - 1, in_mat.cols).clone();
            } else {
                // insert the pixel where it should be
                left_partition = current_src_row.colRange(0, current_coord.col()).clone();
                right_partition = current_src_row.colRange(current_coord.col(), in_mat.cols).clone();
            }

            // grab the color at the last pixel of the left partition
            const cv::Vec3b& left_color = left_partition.at<cv::Vec3b>(left_partition.cols - 1);

            // grab the color at the first pixel of the right partition
            const cv::Vec3b& right_color = right_partition.at<cv::Vec3b>(0);

            // set the new color
            col_insert.at<cv::Vec3b>(0) = cv::Vec3b(
                (left_color[0] + right_color[0]) / 2,
                (left_color[1] + right_color[1]) / 2,
                (left_color[2] + right_color[2]) / 2
            );

            // assemble the merge array
            mats.push_back(left_partition);
            mats.push_back(col_insert);
            mats.push_back(right_partition);

            // merge
            cv::hconcat(mats, res.row(seam[idx].row()));
        }

        res.copyTo(output);
    }

    void Carver::InsertHorizontalSeam(const Seam &seam, cv::InputArray input, cv::OutputArray output) {
        cv::Mat in_mat = input.getMat();
        cv::Mat res = cv::Mat(in_mat.rows + 1, in_mat.cols, in_mat.type());

        // loop through all cols
        for (int idx = 0; idx < res.cols; idx++) {
            // grab the current row and coord
            const cv::Mat &current_src_col = in_mat.col(idx);
            const Coord &current_coord = seam[idx];
            std::vector<cv::Mat> mats;

            // create a 1x1 to insert with the pixel in the input
            cv::Mat row_insert = cv::Mat(1, 1, in_mat.type());
            cv::Mat top_partition, bottom_partition;

            // Insertion is always done such that an inserted pixel has neighbors and can be color corrected
            if (current_coord.row() == 0) {
                // insert the new pixel into row 1
                top_partition = current_src_col.rowRange(0, 1).clone();
                bottom_partition = current_src_col.rowRange(1, in_mat.rows).clone();
            } else if (current_coord.row() == in_mat.rows - 1) {
                // insert the new pixel into index in_mat.rows - 2
                top_partition = current_src_col.rowRange(0, in_mat.rows - 1).clone();
                bottom_partition = current_src_col.rowRange(in_mat.rows - 1, in_mat.rows).clone();
            } else {
                // insert the new pixel wherever it should be
                top_partition = current_src_col.rowRange(0, current_coord.row()).clone();
                bottom_partition = current_src_col.rowRange(current_coord.row(), in_mat.rows).clone();
            }

            // grab the color at the last pixel of the left partition
            const cv::Vec3b& top_color = top_partition.at<cv::Vec3b>(top_partition.rows - 1);

            // grab the color at the first pixel of the right partition
            const cv::Vec3b& bottom_color = bottom_partition.at<cv::Vec3b>(0);

            // set the new color
            row_insert.at<cv::Vec3b>(0) = cv::Vec3b(
                (top_color[0] + bottom_color[0]) / 2,
                (top_color[1] + bottom_color[1]) / 2,
                (top_color[2] + bottom_color[2]) / 2
            );

            // assemble the merge array
            mats.push_back(top_partition);
            mats.push_back(row_insert);
            mats.push_back(bottom_partition);

            // merge
            cv::vconcat(mats, res.col(seam[idx].col()));
        }

        res.copyTo(output);
    }

    void Carver::Carve(const int& rows, const int& cols, cv::InputArray input, cv::OutputArray output) {
        // (0, 0)
        if (rows == 0 && cols == 0) {
            input.copyTo(output);
            return;
        }

        // prepare result
        cv::Mat res;
        input.copyTo(res);

        // Difference between target size and current size.
        // Positive to insert seams, negative to remove.
        int row_diff = rows - res.rows;
        int col_diff = cols - res.cols;

        /** HANDLE ALL CASES WHERE ONE DIMENSION IS POSITIVE */
        // (+, +)
        while (row_diff > 0 && col_diff > 0) {
            const Seam verticalSeam = FindVerticalSeam(res);
            InsertVerticalSeam(verticalSeam, res, res);
            col_diff--; // update the remaining difference

            const Seam horizontalSeam = FindHorizontalSeam(res);
            InsertHorizontalSeam(horizontalSeam, res, res);
            row_diff--; // update the remaining difference
        }

        // (0/-, +)
        while (col_diff > 0) {
            const Seam verticalSeam = FindVerticalSeam(res);
            InsertVerticalSeam(verticalSeam, res, res);
            col_diff--;
        }

        // (+, 0/-)
        while (row_diff > 0) {
            const Seam horizontalSeam = FindHorizontalSeam(res);
            InsertHorizontalSeam(horizontalSeam, res, res);
            row_diff--;
        }

        /** HANDLE ALL CASES WHERE ONE DIMENSION IS NEGATIVE */
        // (-, -)
        while (row_diff < 0 && col_diff < 0) {
            const Seam verticalSeam = FindVerticalSeam(res);
            RemoveVerticalSeam(verticalSeam, res, res);
            col_diff++; // update the remaining difference

            const Seam horizontalSeam = FindHorizontalSeam(res);
            RemoveHorizontalSeam(horizontalSeam, res, res);
            row_diff++; // update the remaining difference
        }

        // (0/+, -)
        while (col_diff < 0) {
            const Seam verticalSeam = FindVerticalSeam(res);
            RemoveVerticalSeam(verticalSeam, res, res);
            col_diff++;
        }

        // (-, 0/+)
        while (row_diff < 0) {
            const Seam horizontalSeam = FindHorizontalSeam(res);
            RemoveHorizontalSeam(horizontalSeam, res, res);
            row_diff++;
        }

        res.copyTo(output);
    }
}

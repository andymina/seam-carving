//
// Created by Andy Mina on 3/7/23.
//

// project
#include <seam_carving/carver.hpp>

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
            cv::Mat col_insert = cv::Mat(
                1, 1, in_mat.type(),
                in_mat.at<cv::Vec3b>(current_coord.row(), current_coord.col())
            );

            /**
             * check order to merge since you can't have jagged matrix.
             * duplicated nodes are always inserted on the right of their original counterpart
             */
            if (current_coord.col() == 0) { // seam inserts (row, 0)
                mats.push_back(col_insert);
                mats.push_back(current_src_row);
            } else if (current_coord.col() == in_mat.cols - 1) { // seam inserts (row, in_mat.cols - 1)
                // insert original row then new col
                mats.push_back(current_src_row);
                mats.push_back(col_insert);
            } else { // seam inserts (row, current_coord.col()
                // get left partition
                const cv::Mat &left_partition = current_src_row.colRange(0, current_coord.col()).clone();
                // get right partition
                const cv::Mat &right_partition = current_src_row.colRange(current_coord.col(), in_mat.cols).clone();
                // insert left, new col, right
                mats.push_back(left_partition);
                mats.push_back(col_insert);
                mats.push_back(right_partition);

                // color correct since i will have a left and right neighbor
                // const cv::Vec3b &left = in_mat.at<cv::Vec3b>(current_coord.row(), current_coord.col() - 1);
                // const cv::Vec3b &right = in_mat.at<cv::Vec3b>(current_coord.row(), current_coord.col());
                // col_insert.at<cv::Vec3b>(0, 0) = (left + right) / 2;
            }

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
            cv::Mat row_insert = cv::Mat(
                    1, 1, in_mat.type(),
                    in_mat.at<cv::Vec3b>(current_coord.row(), current_coord.col())
            );

            // check order to merge since you can't have a jagged matrix
            if (current_coord.row() == 0) { // seam inserts (0, col)
                // insert new row then original col
                mats.push_back(row_insert);
                mats.push_back(current_src_col);
            } else if (current_coord.row() == in_mat.rows - 1) { // seam inserts (in_mat.rows - 1, col)
                // insert original col then new row
                mats.push_back(current_src_col);
                mats.push_back(row_insert);
            } else { // seam inserts (current_coord.row, col)
                // get top partition
                const cv::Mat &top_partition = current_src_col.rowRange(0, current_coord.row()).clone();
                // get bottom partition
                const cv::Mat &bottom_partition = current_src_col.rowRange(current_coord.row(), in_mat.rows).clone();
                // insert top, new, middle
                mats.push_back(top_partition);
                mats.push_back(row_insert);
                mats.push_back(bottom_partition);

                // color correct since i will have top and bottom neighbors
                //const cv::Vec3b &top = in_mat.at<cv::Vec3b>(current_coord.row(), current_coord.col());
                //const cv::Vec3b &bottom = in_mat.at<cv::Vec3b>(current_coord.row() + 1, current_coord.col());
                //row_insert.at<cv::Vec3b>(0, 0) = (top + bottom) / 2;
            }

            // merge
            cv::vconcat(mats, res.col(seam[idx].col()));
        }

        res.copyTo(output);
    }
}

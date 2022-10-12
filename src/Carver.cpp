/**
 * Andy Mina
 */
#include "Carver.h"

namespace SeamCarving {
Seam Carver::FindOptimalVerticalSeam(cv::InputArray img) {
	// compute the energy map
	cv::Mat energy_map;
	Energy::ComputeEnergy(img, energy_map);
	Energy::ComputeVerticalEnergyMap(energy_map, energy_map);

	// find the starting val
	ushort col = 0, min_val = energy_map.at<ushort>(0, col);
	for (int i = 0; i < energy_map.cols; i++)
		if (energy_map.at<ushort>(0, i) < min_val) {
			col = i;
			min_val = energy_map.at<ushort>(0, col);
		}

	Seam seam = Seam(VERT, {});
	int rows = energy_map.rows, cols = energy_map.cols;

	// loop through each row
	for (int row = 0; row < rows; row++) {
		// add the current Coord to the seam
		seam.data.push_back({row, col});

		if (row != rows - 1) {
			// find the direction of min and adjust path
			const ushort &center = energy_map.at<ushort>(row + 1, col);
			const ushort &left = (col - 1 < 0) ?
				std::numeric_limits<ushort>::max() :
				energy_map.at<ushort>(row + 1, col - 1);
			const ushort &right = (col + 1 >= cols) ?
				std::numeric_limits<ushort>::max() :
				energy_map.at<ushort>(row + 1, col + 1);

			// find the min pixel and adjust weight
			int min_energy = std::min({center, left, right});
			if (min_energy == left) col--;
			else if (min_energy == right) col++;
		}
	}

	return seam;
}

Seam Carver::FindOptimalHorizontalSeam(cv::InputArray img) {
	// compute the energy map
	cv::Mat energy_map;
	Energy::ComputeEnergy(img, energy_map);
	Energy::ComputeVerticalEnergyMap(energy_map, energy_map);

	// find the starting val
	ushort row = 0, min_val = energy_map.at<ushort>(row, 0);
	for (int i = 0; i < energy_map.rows; i++)
		if (energy_map.at<ushort>(i, 0) < min_val) {
			row = i;
			min_val = energy_map.at<ushort>(i, 0);
		}


	Seam seam = Seam(HORZ, {});
	int rows = energy_map.rows, cols = energy_map.cols;

	// loop through each col
	for (int col = 0; col < cols; col++) {
		// add the current Coord to the seam
		seam.data.push_back({row, col});

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
			int min_energy = std::min({center, above, below});
			if (min_energy == above) row--;
			else if (min_energy == below) row++;
		}
	}

	return seam;
}

void Carver::RemoveVerticalSeam(const Seam &seam, cv::InputArray in_img, cv::OutputArray out_img) {
	// setup
	cv::Mat in_mat = in_img.getMat();
	cv::Mat res = cv::Mat(in_mat.rows, in_mat.cols - 1, in_mat.type());

	// loop through all rows
	for (int idx = 0; idx < res.rows; idx++) {
		// grab the current row and coord
		const cv::Mat &current_src_row = in_mat.row(idx);
		const Coord &current_coord = seam.data[idx];

		// check where to split the image since you cant have jagged matrix. colRange is exclusive
		if (current_coord.col == 0) { // seam includes (row, 0)
			// copy (row, 1) to (row, in_mat.cols)
			current_src_row.colRange(1, in_mat.cols).copyTo(res.row(idx));
		} else if (current_coord.col == in_mat.cols - 1) { // seam includes (row, in_mat.cols - 1)
			// copy (row, 0) to (row, in_mat.cols - 1)
			current_src_row.colRange(0, in_mat.cols - 1).copyTo(res.row(idx));
		} else { // seam includes (row, current_coord.col)
			// copy (row, 0) to (row, current_coord.col)
			const cv::Mat &left_partition = current_src_row.colRange(0, current_coord.col);
			// copy (row, current_coord.col + 1) to (row, in_mat.cols)
			const cv::Mat &right_partition = current_src_row.colRange(current_coord.col + 1, in_mat.cols);
			// merge the two halves and store into row
			cv::hconcat(left_partition, right_partition, res.row(idx));
		}
	}

	res.copyTo(out_img);
}

void Carver::RemoveHorizontalSeam(const Seam &seam, cv::InputArray in_img, cv::OutputArray out_img) {
	// setup
	cv::Mat in_mat = in_img.getMat();
	cv::Mat res = cv::Mat(in_mat.rows, in_mat.cols - 1, in_mat.type());

	// loop through all cols
	for (int idx = 0; idx < res.cols; idx++) {
		// grab the current col and coord
		const cv::Mat &current_src_col = in_mat.col(idx);
		const Coord &current_coord = seam.data[idx];


		// check where to split the image since you cant have jagged matrix. rowRange is exclusive
		if (current_coord.row == 0) { // seam includes (0, col)
			// copy (1, col) to (in_mat.rows, col)
			current_src_col.rowRange(1, in_mat.rows).copyTo(res.col(idx));
		} else if (current_coord.row == in_mat.rows - 1) { // seam includes (row, in_mat.cols - 1)
			// copy (0, col) to (in_mat.rows - 1, col)
			current_src_col.rowRange(0, in_mat.rows - 1).copyTo(res.col(idx));
		} else { // seam includes (current_coord.row, col)
			// copy (0, col) to (current_coord.row, col)
			const cv::Mat &top_partition = current_src_col.rowRange(0, current_coord.row);
			// copy (current_coord.row + 1, col) to (in_mat.rows, col)
			const cv::Mat &bottom_partition = current_src_col.rowRange(current_coord.row + 1, in_mat.rows);
			// merge the two halves and store into row
			cv::vconcat(top_partition, bottom_partition, res.col(idx));
		}
	}

	res.copyTo(out_img);
}

void Carver::InsertVerticalSeam(const Seam &seam, cv::Mat &img) {
	cv::Mat res = cv::Mat(img.rows, img.cols + 1, img.type());

	for (int idx = 0; idx < seam.data.size(); idx++) {
		const cv::Mat &current_row = img.row(seam.data[idx].row);
		std::vector<cv::Mat> mats;

		cv::Mat col_insert = cv::Mat(1, 1, img.type());
		col_insert.at<cv::Vec3b>(0, 0) =
			img.at<cv::Vec3b>(seam.data[idx].row, seam.data[idx].col);

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
				cv::Vec3b color = cv::Vec3b((left[0] + right[0]) / 2,
											(left[1] + right[1]) / 2,
											(left[2] + right[2]) / 2);
			}
		}
	}

	img = std::move(res);
}

void Carver::InsertHorizontalSeam(const Seam &seam, cv::Mat &img) {
	cv::Mat res = cv::Mat(img.rows, img.cols + 1, img.type());

	for (int idx = 0; idx < seam.data.size(); idx++) {
		const cv::Mat &current_row = img.row(seam.data[idx].col);
		std::vector<cv::Mat> mats;

		cv::Mat col_insert = cv::Mat(1, 1, img.type());
		col_insert.at<cv::Vec3b>(0, 0) =
			img.at<cv::Vec3b>(seam.data[idx].col, seam.data[idx].row);

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
				cv::Vec3b color = cv::Vec3b((left[0] + right[0]) / 2,
											(left[1] + right[1]) / 2,
											(left[2] + right[2]) / 2);
			}
		}
	}

	img = std::move(res);
}

} // namespace SeamCarving
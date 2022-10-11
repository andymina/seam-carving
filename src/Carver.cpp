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

void Carver::RemoveVerticalSeam(const Seam &seam, cv::Mat &img) {
	cv::Mat res = cv::Mat(img.rows, img.cols - 1, img.type());

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
			cv::hconcat(current_row.colRange(0, seam.data[idx].col),
						current_row.colRange(seam.data[idx].col + 1, img.cols),
						res.row(idx));
		}
	}

	img = std::move(res);
}

void Carver::RemoveHorizontalSeam(const Seam &seam, cv::Mat &img) {
	cv::Mat res = cv::Mat(img.rows - 1, img.cols, img.type());

	for (int idx = 0; idx < seam.data.size(); idx++) {
		const cv::Mat &current_row = img.row(seam.data[idx].col);

		if (seam.data[idx].row == 0) {
			// exclude first val
			current_row.colRange(1, img.cols)
				.copyTo(res.row(seam.data[idx].col));
		} else if (seam.data[idx].row == img.cols - 1) {
			// exclude last val
			current_row.colRange(0, img.cols - 1)
				.copyTo(res.row(seam.data[idx].col));
		} else {
			// merge two halves
			cv::hconcat(current_row.colRange(0, seam.data[idx].row),
						current_row.colRange(seam.data[idx].row + 1, img.cols),
						res.row(seam.data[idx].col));
		}
	}

	img = std::move(res);
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
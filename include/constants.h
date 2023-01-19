/**
 * Andy Mina
 *
 * Helper file that define constants to be used throughout the project.
 */
#ifndef constants_h
#define constants_h

#include <vector>
#include <nlohmann/json.hpp>
#include <opencv2/core.hpp>

namespace SeamCarving {
/** Helper representation of a pixel coordinate in an image in terms of (row, col). */
struct Coord {
	int row, col;

	Coord(const int& row_ = 0, const int& col_ = 0):
		row{row_}, col{col_}
		{ }

	/**
	 * Determines if two Coords are equal based on their rows and cols.
	 * 
	 * @param rhs - the coord to compare
	 * @returns true if the Coords are equal, otherwise false
	*/
	const bool operator==(const Coord& rhs) const {
		return row == rhs.row && col == rhs.col;
	}

	// define to_json/from_json based on member names
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Coord, row, col);
};


/** A Enum to help better describe what direction to use in functions and seams. */
enum Dir { VERT, HORZ };
// define to_json/from_json
NLOHMANN_JSON_SERIALIZE_ENUM(Dir, {
    { VERT, "VERT" },
    { HORZ, "HORZ" }
})

/** Helper class to describe a seam. */
struct Seam {
	Dir dir;
	std::vector<Coord> data;

	/**
	 * Constructs a Seam using the specified data.
	 * 
	 * @param dir - the direction of the seam
	 * @param data - a vector of the pixels in the seam specified by Coord
	*/
	Seam(const Dir& dir_ = VERT, const std::vector<Coord>& data_ = {}):
		dir{dir_}, data{data_}
		{ }

	/**
	 * Determines if two seams are equivalent based on their direction, size, and data vectors.
	 * 
	 * @param rhs - the other Seam
	 * @returns true if the seams are equal, false otherwise
	*/
	const bool operator==(const Seam& rhs) const {
		return (dir == rhs.dir) && (data.size() == rhs.data.size()) && (data == rhs.data); 
	}

	// define to_json/from_json based on member names
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Seam, dir, data);
};

/** Matrix Utils */
// not needed
void to_json(nlohmann::json& j, const cv::Mat& matrix) { }

/**
 * Reads in a matrix from JSON. Mainly used for reading in test matrices.
 * 
 * @remarks the matrix read in will be <int>
*/
void from_json(const nlohmann::json& j, cv::Mat& matrix) {
	std::vector<int> data = j["data"];
	matrix = cv::Mat(j["rows"], j["cols"], CV_32S, data.data());
}

} // namespace SeamCarving

#endif /* constants_h */
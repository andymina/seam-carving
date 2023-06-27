//
// Created by Andy Mina on 3/22/23.
//

#ifndef SEAM_CARVING_PARSER_HPP
#define SEAM_CARVING_PARSER_HPP

#include <nlohmann/json.hpp>

#include <fstream>
#include <string>
#include <cstddef>
#include <sstream>
#include <stdexcept>

/**
 * Parses Seam Carving test CSVs as exported by Google Sheets. The helper functions split the lines on
 * CRLF (\r\n), because Google Sheets exports like that, and then splits on commas to get each value. In addition,
 * all helper functions assume that file_ points to the first line after the section label for the data structure to
 * be parsed. For example, `parseMatrix` assumes file_ points to the first line (TYPE) after the label (MATRIX).
 */
class Parser {
private:
    nlohmann::ordered_json parsed_;
    std::ifstream file_;
    std::string line_;
    std::size_t rows_, cols_, coord_range_;

    struct FileHeaders {
        int matrix_rows;
        int matrix_cols;
        int seam_coord_range;
    };

    /**
     * Parses a CSV matrix of `rows_ * cols_` size to JSON.
     *
     * @param parent_label the parent label to attach the data to in the JSON; if "", attaches the matrix onto the root
     * object
     * @param label the label of the matrix in the JSON
     */
    void ParseMatrix(
            const std::size_t& rows, const std::size_t& cols,
            const std::string& label, const std::string& parent_label = "");

    /**
     * Parses the OpenCV type of a CSV matrix.
     * @return the type as represented by an integer.
     */
    int ParseMatrixType();

    /**
     * Parses a CSV seam to JSON.
     *
     * @details
     * Seam coordinates are stored as floats in the Google Sheet for simplicity. The integer
     * part of the float is the row. The fractional part of the float is the col. Once the value
     * is parsed as float, it can be split into its respective parts using `std::modf()`. This
     * approach is limited by the range of possible row and col values. To convert the fractional
     * part back to an integer it must be multiplied by a factor of 10, namely `coord_range_`.
     * Currently coord_range_ is 100 so this approach supports values from 0-99. Should the tests
     * ever expand past 10x10 size, then coord_range_ will need to be 1000 to cover the next values.
     *
     *
     * @param direction The direction of the seam being parsed. One of ["VERT", "HORZ"].
     * @param label The label/key of the matrix in the JSON.
     * @param parent_label The parent label to attach the data to in the JSON; if "", attaches
     * the matrix onto the root object.
     */
    void ParseSeam(const std::string& direction, const std::string& label, const std::string& parent_label = "");

    /** HELPER FUNCTIONS */

    /**
     * Parses the information headers at the top of a seam carving test data CSV file.
     *
     * @return the headers of the file
     */
    FileHeaders ParseFileHeaders();

    /**
     * Parses a CSV file filled with seam carving test data to JSON.
     */
    void ParseFile();

    /**
     * Converts a string representation of OpenCV matrix type enum to its corresponding integer.
     *
     * @param type the string representation of an OpenCV matrix type in enum form (CV_8UC1, CV_16SC3, etc.)
     * @return an integer of representation of the enum passed as a string
     */
    static int StringToCVType(const std::string& type);

public:
    Parser(const std::string& path, const int& test_id);
    ~Parser();
    inline const nlohmann::ordered_json& GetJSON() { return parsed_; };
};

#endif //SEAM_CARVING_PARSER_HPP

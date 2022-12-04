#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <cmath>
#include <sstream>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace fs = std::__fs::filesystem;
using json = nlohmann::ordered_json;

class Parser {
    private:
        json parsed_;
        std::string line_;
        std::string file_path_;

        /**
         * Given a TSV file where the cursor points to the first value of a one-dimensional array of
         * size `rows * cols` matrix, adds the parsed matrix under the corresponding parent_label
         * and label in `this->parsed_`.
         * 
         * @remarks
         * The file should be a TSV exported from Google Sheets. The function splits the lines on
         * CRLF (\r\n), because Google Sheets exports like that, and then splits on tabs (\t) to get
         * each value.
         * 
         * @param file - the TSV file to read from
         * @param parent_label - the parent label to attach the data to in the JSON; if "", attaches
         * the matrix onto the root object
         * @param label - the label of the matrix in the JSON
         * @param rows - the number of rows in the matrix
         * @param cols - the number of cols in the matrix
        */
        void parseMatrix(
            std::ifstream& file,
            const std::string& parent_label,
            const std::string& label,
            const int& rows, const int& cols
        ) {
            std::vector<int> res;
            int count = 0, size = rows * cols;
            while (count < size && std::getline(file, this->line_, '\r')) {
                std::stringstream ss(this->line_);
                while (std::getline(ss, this->line_, '\t')) {
                    if (this->line_.size() != 0) {
                        res.push_back(std::stoi(this->line_));
                        count++;
                    }
                }
            }

            if (parent_label == "") this->parsed_[label] = res;
            else this->parsed_[parent_label][label] = res;
        }

        /**
         * Given a TSV file where the cursor points to the first value of a one-dimensional array
         * of size `size`, adds the parsed seam under the corresponding parent_label and label
         * in `this->parsed_`.
         * 
         * @remarks
         * The file should be a TSV exported from Google Sheets. The function splits the lines on
         * CRLF (\r\n), because Google Sheets exports like that, and then splits on tabs (\t) to get
         * each value.
         * Seam coordinates are stored as floats in the Google Sheet for simplicity. The integer
         * part of the float is the row. The fractional part of the float is the col. Once the value
         * is parsed as float, it can be split into its respective parts using `std::modf()`. This
         * approach is limited by the range of possible row and col values. To convert the fractional
         * part back to an integer it must be multiplied by a factor of 10, namely `coord_range`.
         * Currently coord_range is 100 so this approach supports values from 0-99. Should the tests
         * ever expand past 10x10 size, then coord_range will need to be 1000 to cover the next values.
         * 
         * @param file - the TSV file to read from
         * @param parent_label - the parent label to attach the data to in the JSON; if "", attaches
         * the matrix onto the root object
         * @param label - the label of the matrix in the JSON
         * @param size - the length of the seam
         * @param coord_range - the coordinate range as described in the @remarks
        */
        void parseSeam(
            std::ifstream& file,
            const std::string& parent_label,
            const std::string& label,
            const int& size,
            const int& coord_range
        ) {
            std::getline(file, this->line_, '\r');
            std::stringstream ss(this->line_);
            std::vector<json> res;
            int count = 0;

            while (count < size && std::getline(ss, this->line_, '\t')) {
                if (this->line_.size() != 0) {
                    double coord = std::stod(this->line_);
                    double row;
                    double col = std::modf(coord, &row) * coord_range;
                 
                    res.push_back({
                        { "row", int(round(row)) },
                        { "col", int(round(col)) }
                    });
                    count++;
                }
            }
            
            if (parent_label == "") this->parsed_[label] = res;
            else this->parsed_[parent_label][label] = res;
        }

    public:
        /**
         * Given a file path and a test_id, constructs a Parser to read the test TSV files as
         * exported from Google Sheets.
         * 
         * @param path - the file path
         * @param test_id - the test_id of the current file 
        */
        Parser(const std::string& path, const int& test_id): file_path_{ path } {
            this->parsed_["test_id"] = test_id;
        }

        /**
         * Parses a TSV file filled with seam carving test data as exported by Google Sheet.
         * 
         * @param rows - the number of rows in each matrix of this file
         * @param cols - the number of cols of each matrix of this file
         * @param coord_range - the coordinate range for Seam as described in the @remarks of
         * `this->parseSeam()`
        */
        void parseFile(const int& rows, const int& cols, const int& coord_range) {
            std::ifstream file(this->file_path_);
            while (std::getline(file, this->line_, '\r')) {
                // input
                if (this->line_.find("MATRIX") != std::string::npos) {
                    this->parseMatrix(file, "", "input", rows, cols);  
                }

                // energy
                else if (this->line_.find("SOBEL") != std::string::npos) {
                    this->parseMatrix(file, "energy", "sobel", rows, cols);
                } else if (this->line_.find("VERT MAP") != std::string::npos) {
                    this->parseMatrix(file, "energy", "map - vertical", rows, cols);
                } else if (this->line_.find("HORZ MAP") != std::string::npos) {
                    this->parseMatrix(file, "energy", "map - horizontal", rows, cols);
                }

                // seams
                else if (this->line_.find("VERT SEAM") != std::string::npos) {
                    this->parseSeam(file, "carver", "find - vertical", cols, coord_range);
                } else if (this->line_.find("HORZ SEAM") != std::string::npos) {
                    this->parseSeam(file, "carver", "find - horizontal", rows, coord_range);
                }

                // removes
                else if (this->line_.find("REMOVE VERT") != std::string::npos) {
                    this->parseMatrix(file, "carver", "remove - vertical", rows, cols - 1);
                } else if (this->line_.find("REMOVE HORZ") != std::string::npos) {
                    this->parseMatrix(file, "carver", "remove - horizontal", rows - 1, cols);
                }
                
                // inserts
                else if (this->line_.find("INSERT VERT") != std::string::npos) {
                    this->parseMatrix(file, "carver", "insert - vertical", rows, cols + 1);
                } else if (this->line_.find("INSERT HORZ") != std::string::npos) {
                    this->parseMatrix(file, "carver", "insert - horizontal", rows + 1, cols);
                }
            }

            file.close();
        }

        const json& getJSON() {
            return this->parsed_;
        };
};

int main() {
    // count the number of seam carving tests
    int test_count = 0;
    std::string test_data_dir = "test/raw-data/";
    for (const auto& entry : fs::directory_iterator(test_data_dir)) {
        std::string path = entry.path();
        if (path.find("Seam Carving") != std::string::npos) {
            test_count++;
        }
    }
    
    // test parsing setup
    const int MATRIX_ROWS = 6;
    const int MATRIX_COLS = 6;
    const int COORD_RANGE = 100;
    json data;

    // file iteration setup
    std::string file_prefix = test_data_dir + "Seam Carving Tests - TestId_";
    std::string file_suffix = ".tsv";

    // parse test files
    for (int test_id = 0; test_id < test_count; test_id++) {
        std::string file_name = file_prefix + std::to_string(test_id) + file_suffix;
        Parser p(file_name, test_id);
        p.parseFile(MATRIX_ROWS, MATRIX_COLS, COORD_RANGE);
        data.push_back(p.getJSON());
    }

    std::ofstream out_file("test/data.json");
    out_file << data.dump(4);
}
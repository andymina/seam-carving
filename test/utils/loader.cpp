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
        Parser(const std::string& path, const int& test_id): file_path_{ path } {
            this->parsed_["test_id"] = test_id;
        }

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

    for (int test_id = 0; test_id < test_count; test_id++) {
        std::string file_name = file_prefix + std::to_string(test_id) + file_suffix;
        Parser p(file_name, test_id);
        p.parseFile(MATRIX_ROWS, MATRIX_COLS, COORD_RANGE);
        data.push_back(p.getJSON());
    }

    std::ofstream out_file("test/data.json");
    out_file << data.dump(4);
}
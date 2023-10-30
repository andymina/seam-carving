//
// Created by Andy Mina on 3/22/23.
//

#include "parser.hpp"

void Parser::ParseMatrix(
        const std::size_t& rows, const std::size_t& cols,
        const std::string& label, const std::string& parent_label) {
    int type = ParseMatrixType();

    std::vector<int> res;
    int elements_seen = 0;
    int matrix_size = rows * cols;

    // read in matrix as a one-dimensional vector
    while (elements_seen < matrix_size && std::getline(file_, line_)) {
        std::stringstream ss(line_);
        while (std::getline(ss, line_, ',')) {
            // add the element if we hit a value
            if (!line_.empty() && line_ != "\r") {
                res.push_back(std::stoi(line_));
                ++elements_seen;
            }
        }
    }

    nlohmann::ordered_json output = {
            {"rows", rows},
            {"cols", cols},
            {"type", type},
            {"data",  res}
    };

    if (parent_label.empty()) this->parsed_[label] = output;
    else this->parsed_[parent_label][label] = output;
}

int Parser::ParseMatrixType() {
    // throw away section header
    std::getline(file_, line_);

    std::stringstream ss(line_);

    // throw away type label
    std::getline(ss, line_, ',');

    // grab depth value
    std::getline(ss, line_, ',');

    return StringToCVType(line_);
}

void Parser::ParseSeam(const std::string& direction, const std::string& label, const std::string& parent_label) {
    int size = (direction == "VERT") ? rows_ : cols_;

    std::getline(file_, line_);
    std::stringstream ss(line_);
    std::vector<nlohmann::ordered_json> data;
    int count = 0;

    while (count < size && std::getline(ss, line_, ',')) {
        if (!line_.empty()) {
            double coord = std::stod(line_);
            double row;
            double col = std::modf(coord, &row) * coord_range_;

            data.push_back({
               {"row", int(round(row))},
               {"col", int(round(col))}
            });
            count++;
        }
    }

    nlohmann::ordered_json seam = {
        {"dir", direction},
        {"coords", data}
    };

    if (parent_label.empty()) this->parsed_[label] = seam;
    else this->parsed_[parent_label][label] = seam;
}

Parser::FileHeaders Parser::ParseFileHeaders() {
    // throw the first line away since it's the section divider
    std::getline(file_, line_);

    // header set up
    int count = 0, header_size = 3;

    // read headers
    std::vector<int> header_vals;
    while (count < header_size && std::getline(file_, line_)) {
        std::stringstream ss(line_);
        while (std::getline(ss, line_, ',')) {
            if (!line_.empty() && isnumber(line_[0])) {
                header_vals.push_back(std::stoi(line_));
                count++;
            }
        }
    }

    // set headers
    return {
            header_vals[0],
            header_vals[1],
            header_vals[2]
    };
}

void Parser::ParseFile() {
    // loop through the file
    while (std::getline(file_, line_)) {
        // input
        if (line_.find("MATRIX") != std::string::npos) {
            ParseMatrix(rows_, cols_, "input");
        }

        // energy
        else if (line_.find("SOBEL") != std::string::npos) {
            ParseMatrix(rows_, cols_, "sobel", "energy");
        } else if (line_.find("VERT MAP") != std::string::npos) {
            ParseMatrix(rows_, cols_, "map - vertical", "energy");
        } else if (line_.find("HORZ MAP") != std::string::npos) {
            ParseMatrix(rows_, cols_, "map - horizontal", "energy");
        }

        // find seams
        else if (line_.find("VERT SEAM") != std::string::npos) {
            ParseSeam("VERT", "find - vertical", "carver");
        } else if (line_.find("HORZ SEAM") != std::string::npos) {
            ParseSeam("HORZ", "find - horizontal", "carver");
        }

        // removes
        else if (line_.find("REMOVE VERT") != std::string::npos) {
            ParseMatrix(rows_, cols_ - 1, "remove - vertical", "carver");
        } else if (line_.find("REMOVE HORZ") != std::string::npos) {
            ParseMatrix(rows_ - 1, cols_, "remove - horizontal", "carver");
        }

        // inserts
        else if (line_.find("INSERT VERT") != std::string::npos) {
            ParseMatrix(rows_, cols_ + 1, "insert - vertical", "carver");
        } else if (line_.find("INSERT HORZ") != std::string::npos) {
            ParseMatrix(rows_ + 1, cols_, "insert - horizontal", "carver");
        }
    }
}

int Parser::StringToCVType(const std::string &type) {
    if (type.substr(0, 3) != "CV_")
        throw std::invalid_argument(
                "stringToCVType parameter must be a string representing one of OpenCV's types.");

    // start from idx 3 to skip "CV_"
    std::size_t position_of_split = type.substr(3).find('C');
    int channels = 1;
    if (position_of_split != std::string::npos) {
        // extra +3 here since the position is relative to starting from index 3
        std::string channel_str = type.substr(position_of_split + 3 + 1);
        channels = std::stoi(channel_str);
    }

    /**
     * The base value for a type is given by it's numerical representation with one channel.
     * More here: https://stackoverflow.com/a/39780825
     */
    int depth_base_value = 0;
    std::string depth_str = type.substr(3, position_of_split);
    if (depth_str == "8U") depth_base_value = 0;
    else if (depth_str == "8S") depth_base_value = 1;
    else if (depth_str == "16U") depth_base_value = 2;
    else if (depth_str == "16S") depth_base_value = 3;
    else if (depth_str == "32S") depth_base_value = 4;
    else if (depth_str == "32F") depth_base_value = 5;
    else if (depth_str == "64F") depth_base_value = 6;

    return depth_base_value + ((channels - 1) << 3);
}

Parser::Parser(const std::string &path, const int &test_id) {
    parsed_["test_id"] = test_id;
    file_ = std::ifstream(path);

    FileHeaders headers = ParseFileHeaders();
    rows_ = headers.matrix_rows;
    cols_ = headers.matrix_cols;
    coord_range_ = headers.seam_coord_range;

    ParseFile();
}

Parser::~Parser() {
    file_.close();
}

//
// Created by Andy Mina on 3/7/23.
//

#include "parser.hpp"

// 3rd party
#include <nlohmann/json.hpp>

// std
#include <iostream>
#include <iterator>
#include <string>
#include <fstream>
#include <filesystem>

namespace fs = std::__fs::filesystem;

int main() {
    // count the number of seam carving tests
    std::string test_data_dir = "tests/data/";
    int test_count = std::distance(fs::directory_iterator(test_data_dir), fs::directory_iterator{});

    std::string file_prefix = test_data_dir + "Seam Carving Tests - TestId_";
    std::string file_ext = ".csv";
    std::string file_name;
    nlohmann::ordered_json data;

    // parse test files
    for (int test_id = 0; test_id < test_count; ++test_id) {
        // build file name
        file_name = file_prefix + std::to_string(test_id) + file_ext;
        Parser p(file_name, test_id);
        const nlohmann::ordered_json& parsed = p.GetJSON();
        data.push_back(parsed);
        file_name = "";
    }

    std::ofstream out_file("tests/data.json");
    out_file << data.dump(4);
}

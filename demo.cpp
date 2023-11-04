/**
 * Andy Mina
 *
 * File to test SeamCarver in C++ implementation. Given a path to an image, the target number of
 * rows, and the target number of cols, carves the image and displays the result.
 *
 * Args:
 *    {path to img}
 *    {target rows}
 *    {target cols}
*/
#include "timer.hpp"
#include <seam_carving/carver.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>

namespace sc = seam_carving;

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " {path to img} {target cols} {target rows}\n";
        return -1;
    }

    const std::string input_path(argv[1]);
    const int target_cols(std::stoi(argv[2]));
    const int target_rows(std::stoi(argv[3]));

    cv::Mat img = cv::imread(input_path);
    std::cout << "Original size: " << img.cols << "x" << img.rows << "\n";

    cv::Mat res = img;
    Timer carver_timer;

    std::cout << "Starting carving...\n";

    carver_timer.tick();
    sc::Carver::Carve(target_rows, target_cols, img, res);
    carver_timer.tock();

    std::cout << "Carved size: " << res.cols << "x" << res.rows << "\n";
    std::cout << "Carving took " << carver_timer.duration() << " ms\n";

    cv::imshow("Original", img);
    cv::imshow("Carved", res);
    int k = cv::waitKey(0);
    return 0;
}

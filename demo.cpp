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
#include <seam_carving/carver.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>
#include <chrono>

namespace sc = seam_carving;

template <class Duration = std::chrono::milliseconds, class Clock = std::chrono::steady_clock>
class Timer {
    using time_pt = typename Clock::time_point;

    time_pt _start = Clock::now();
    time_pt _end = {};

public:
    void tick() { _start = Clock::now(); }
    void tock() { _end = Clock::now(); }

    template <class T = Duration>
    auto duration() const {
        return std::chrono::duration_cast<T>(_end - _start).count();
    }
};

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

    cv::Mat res;
    Timer carver_timer;

    std::cout << "Starting carving...\n";

    carver_timer.tick();
    sc::Carver::Carve(target_rows, target_cols, img, res);
    carver_timer.tock();

    std::cout << "Carving took " << carver_timer.duration() << " ms\n";

    std::cout << "Carved size: " << res.cols << "x" << res.rows << "\n";

    cv::imshow("Original", img);
    cv::imshow("Carved", res);
    int k = cv::waitKey(0);
    return 0;
}

//int main() {
//    const std::string input_path = "samples/dali.jpeg";
//
//    cv::Mat img = cv::imread(input_path);
//    std::cout << "Original size: " << img.cols << "x" << img.rows << "\n";
//
//    cv::Mat res;
//
//    sc::Carver::InsertVerticalSeam(sc::Carver::FindVerticalSeam(img), img, res);
//
//    std::cout << "Carved size: " << res.cols << "x" << res.rows << "\n";
//
//    cv::imshow("Original", img);
//    cv::imshow("Carved", res);
//    int k = cv::waitKey(0);
//    return 0;
//}

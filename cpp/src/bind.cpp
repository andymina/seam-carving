#include <pybind11/pybind11.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

namespace py = pybind11;

void kill(std::string winname) {
  cv::destroyWindow(winname);
}

void show(std::string path) {
  cv::Mat img = cv::imread(path);
  cv::namedWindow("Simple Demo", cv::WINDOW_AUTOSIZE);
  cv::imshow("Simple Demo", img);
  int k = cv::waitKey(0);
}

PYBIND11_MODULE(module_name, handle) {
  handle.doc() = "BOOO";
  handle.def("show", &show);
  handle.def("kill", &kill);
}
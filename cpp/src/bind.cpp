#include <pybind11/pybind11.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

namespace py = pybind11;

void kill(std::string winname) {
  cv::destroyWindow(winname);
}

void show(std::string path) {
  cv::Mat img = cv::imread(path);
  cv::imshow("Display Window", img);
  int k = cv::waitKey(0);
}  

PYBIND11_MODULE(seam_carving, handle) {
  handle.doc() = "BOOO";
  handle.def("show", &show);
  handle.def("kill", &kill);
}
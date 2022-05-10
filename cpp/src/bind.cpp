#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "constants.h"
#include "CarvableImage.h"

namespace py = pybind11;
namespace sc = SeamCarving;

PYBIND11_MODULE(seam_carving, module) {
  module.doc() = "Seam Carving module made by Andy Mina.";

  /** expose enums */
  py::enum_<sc::Dir>(module, "Dir")
    .value("VERT", sc::Dir::VERT)
    .value("HORZ", sc::Dir::HORZ)
    .export_values();

  py::enum_<sc::ImageType>(module, "ImageType")
    .value("ORIGINAL", sc::ImageType::ORIGINAL)
    .value("TRANSPOSE", sc::ImageType::TRANSPOSE)
    .value("RESULT", sc::ImageType::RESULT)
    .value("ENERGY", sc::ImageType::ENERGY)
    .value("VERT_MAP", sc::ImageType::VERT_MAP)
    .value("HORZ_MAP", sc::ImageType::HORZ_MAP)
    .export_values();

  /** expose classes */
  // py::class_<sc::CarvableImage>(module, "SeamCarver")
  //   .def(py::init<const std::string &>());
  py::class_<sc::CarvableImage>(module, "SeamCarver")
    .def(py::init<const std::string &>())
    .def("export", &sc::CarvableImage::Export);
}
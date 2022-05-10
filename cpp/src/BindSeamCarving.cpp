#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "CarvableImage.h"

namespace py = pybind11;
namespace sc = SeamCarving;

PYBIND11_MODULE(seam_carving, handle) {
  py::class_<sc::CarvableImage>(handle, "CarvableImage")
    .def(py::init<std::string>())
    .def()
}
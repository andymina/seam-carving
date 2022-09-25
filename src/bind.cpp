/**
 * Andy Mina
 * 
 * Defines the bindings for the Python seam carving module.
*/
// external
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// proj
#include "constants.h"
#include "CarvableImage.h"

namespace py = pybind11;
namespace sc = SeamCarving;

/** 
 * Macro function that defines a Python Module in runtime
 * 
 * @param seam_carving the name of the module to be outputted
 * @param module the internal C++ reference to the module for bindings
 * 
 * TODO: add documentation for all functions
*/
PYBIND11_MODULE(seam_carving, module) {
  module.doc() = "Seam Carving module made by Andy Mina for CSCI 49369.";

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
  py::class_<sc::Coord>(module, "Coord")
    .def(py::init<const int &, const int &>());
  py::class_<sc::Seam>(module, "Seam")
    .def(py::init<const sc::Dir &, const std::vector<sc::Coord>  &>());

  /** TODO: expose Get images methods by converting cv::Mat to np.ndarray */
  py::class_<sc::CarvableImage>(module, "SeamCarver")
    .def(py::init<const std::string &>())
    .def("rows", &sc::CarvableImage::num_rows)
    .def("cols", &sc::CarvableImage::num_cols)

    .def("carve", &sc::CarvableImage::SeamCarve)

    .def("find_seam", &sc::CarvableImage::FindOptimalSeam)
    .def("find_k_seams", &sc::CarvableImage::FindKOptimalSeams)
    .def("insert_seam", &sc::CarvableImage::InsertSeam)
    .def("insert_k_seams", &sc::CarvableImage::InsertKSeams)
    .def("remove_seam", &sc::CarvableImage::RemoveSeam)

    .def("highlight_seam", &sc::CarvableImage::HighlightSeam,
      py::arg("seam"), py::arg("r") = 176, py::arg("g") = 38, py::arg("b") = 255)  
    .def("highlight_k_seams", &sc::CarvableImage::HighlightKSeams,
      py::arg("seam"), py::arg("r") = 176, py::arg("g") = 38, py::arg("b") = 255)

    .def("export", &sc::CarvableImage::Export)
    .def("reset", &sc::CarvableImage::Reset);
}
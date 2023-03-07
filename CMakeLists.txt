cmake_minimum_required(VERSION 3.25)
project(seam_carving)

set(CMAKE_CXX_STANDARD 17)

# 3rd party set up
set(
    OPENCV_LIBS
        opencv_core opencv_imgcodecs opencv_highgui opencv_imgproc
    CACHE INTERNAL ""
)
set(
    PYBIND11_LIBS
        pybind11::lto pybind11::module
    CACHE INTERNAL ""
)
set(
    JSON_LIBS
        nlohmann_json::nlohmann_json
    CACHE INTERNAL ""
)
set(
    GTEST_LIBS
        GTest::gtest_main
    CACHE INTERNAL ""
)

add_subdirectory(libseam_carving)
include(Demo.cmake)

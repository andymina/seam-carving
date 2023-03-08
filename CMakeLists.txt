cmake_minimum_required(VERSION 3.25)
project(seam_carving)

set(CMAKE_CXX_STANDARD 17)

# external library vars
set(OPENCV_LIBS
    opencv_core opencv_imgcodecs opencv_highgui opencv_imgproc
    CACHE INTERNAL
    "OpenCV libraries necessary for libseam_carving"
)
set(PYBIND11_LIBS
    pybind11::lto pybind11::module
    CACHE INTERNAL
    "pybind11 libraries necessary for python bindings"
)
set(JSON_LIBS
    nlohmann_json::nlohmann_json
    CACHE INTERNAL
    "JSON libraries needed to test libseam_carving"
)
set(GTEST_LIBS
    GTest::gtest_main GTest::gtest
    CACHE INTERNAL
    "GoogleTest libraries needed to test libseam_carving"
)

# dir shortcuts
set(CMAKE_MODULE_DIR
    ${PROJECT_SOURCE_DIR}/cmake
    CACHE INTERNAL
    "Folder containing all .cmake files"
)
set(LIBRARY_ROOT_DIR
    ${PROJECT_SOURCE_DIR}/libseam_carving
    CACHE INTERNAL
    "Root directory for libseam_carving"
)
set(LIBRARY_TEST_DIR
    ${LIBRARY_ROOT_DIR}/tests
    CACHE INTERNAL
    "Root directory for libseam_carving tests"
)

# find all packages ahead of time
find_package(OpenCV CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

add_subdirectory(libseam_carving)
include(${CMAKE_MODULE_DIR}/demo.cmake)

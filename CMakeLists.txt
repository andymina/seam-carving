# project set up
cmake_minimum_required(VERSION 3.4)
project(seam-carving)
set(CMAKE_CXX_STANDARD 14)

# opencv
set(OpenCV_DIR /Users/andymina/Coding/libs/opencv) # path to opencv dir
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})

# pybind
set(PYBIND11_FINDPYTHON ON)
add_subdirectory(deps/pybind11) # path to pybind11

# build
pybind11_add_module(module_name SHARED cpp/src/bind.cpp) # path to cpp bindings
target_link_libraries(module_name PRIVATE ${OpenCV_LIBS})
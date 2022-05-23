# project set up
cmake_minimum_required(VERSION 3.4)
project(seam-carving)
set(CMAKE_CXX_STANDARD 14)
set(PROJ_INCLUDE_DIR cpp/include)

# opencv
set(OpenCV_DIR /Users/andymina/Coding/libs/opencv) # path to opencv dir, build from source with option -DBUILD_ZLIB=OFF on mac
find_package(OpenCV REQUIRED)
link_directories(${OpenCV_LIBRARY_DIRS})

# pybind
include_directories(${OpenCV_INCLUDE_DIRS})
set(PYBIND11_FINDPYTHON ON)
add_subdirectory(deps/pybind11) # path to pybind11, installed by project

# build
include_directories(${PROJ_INCLUDE_DIR})

# off for C++ standalone
pybind11_add_module(
  seam_carving MODULE
  cpp/src/bind.cpp cpp/src/CarvableImage.cpp cpp/src/energy.cpp # path to src files
)

# on for C++ standalone
# add_executable(
#   seam_carving
#   cpp/src/main.cpp cpp/src/CarvableImage.cpp cpp/src/energy.cpp
# )

target_link_libraries(seam_carving PRIVATE ${OpenCV_LIBS})

# off for C++ standalone
set_target_properties(
  seam_carving
  PROPERTIES
    LIBRARY_OUTPUT_NAME seam_carving  # module output name
    LIBRARY_OUTPUT_DIRECTORY ../      # output directory
    SUFFIX .so
)
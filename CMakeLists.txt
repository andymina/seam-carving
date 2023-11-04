cmake_minimum_required(VERSION 3.25)
project(seam_carving)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# find all packages ahead of time
find_package(OpenCV CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(plog CONFIG REQUIRED)

add_subdirectory(libseam_carving)

enable_testing()
include(GoogleTest)
add_subdirectory(tests)

# demo
add_executable(demo demo.cpp)
target_include_directories(demo
        PRIVATE
        ${LIBRARY_TEST_DIR}/include)
target_link_libraries(demo
        PRIVATE
        seam_carving ${OPENCV_LIBS})

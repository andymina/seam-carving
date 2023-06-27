cmake_minimum_required(VERSION 3.25)
project(seam_carving)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

message("${CMAKE_CXX_COMPILER_ID} - COMPILER HERE")

# find all packages ahead of time
find_package(OpenCV CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(plog CONFIG REQUIRED)

add_subdirectory(libseam_carving)

enable_testing()
include(GoogleTest)
add_subdirectory(tests)

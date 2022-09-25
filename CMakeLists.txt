# project setup
cmake_minimum_required(VERSION 3.4)
set(
    CMAKE_TOOLCHAIN_FILE
    "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file"
)
project(seam-carving)

# find packages
find_package(OpenCV CONFIG REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

# link libs
target_link_libraries(
    ${PROJECT_NAME}
    opencv_core
    pybind11::lto pybind11::module
)


message("${VCPKG_TARGET_TRIPLET} -- hello")
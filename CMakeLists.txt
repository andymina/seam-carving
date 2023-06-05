cmake_minimum_required(VERSION 3.25)
#set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
project(seam_carving)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# find all packages ahead of time
find_package(OpenCV CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

#add_subdirectory(libseam_carving)
#include(${CMAKE_MODULE_DIR}/demo.cmake)
set(LIBRARY_SOURCE_FILES
        ${LIBRARY_ROOT_DIR}/energy.cpp
        ${LIBRARY_ROOT_DIR}/seam.cpp
        ${LIBRARY_ROOT_DIR}/carver.cpp
        CACHE INTERNAL
        "Source files for libseam_carving")

add_library(seam_carving SHARED)
target_sources(seam_carving PRIVATE ${LIBRARY_SOURCE_FILES})
target_include_directories(seam_carving
        PUBLIC
        ${LIBRARY_ROOT_DIR})
#target_link_libraries(seam_carving
#        PRIVATE
#        ${OPENCV_LIBS} ${JSON_LIBS})

set(OPENCV_LIBS opencv_core opencv_imgcodecs opencv_highgui opencv_imgproc)
target_link_libraries(seam_carving
        PUBLIC
#        opencv_core opencv_imgcodecs opencv_highgui opencv_imgproc
        ${OPENCV_LIBS}
        )

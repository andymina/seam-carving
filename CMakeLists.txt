cmake_minimum_required(VERSION 3.25)
project(seam_carving)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

if(CMAKE_CXX_COMPILER_LOADED)
    message(STATUS "Compiler path: ${CMAKE_CXX_COMPILER}")
    message(STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
    message(STATUS "Compiler version:
             ${CMAKE_CXX_COMPILER_VERSION}")
    message(STATUS "Compiler is part of GCC:
             ${CMAKE_COMPILER_IS_GNUCXX}")
    message(STATUS "CXX Flags: ${CMAKE_CXX_FLAGS}")
endif()

# find all packages ahead of time
find_package(OpenCV CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(plog CONFIG REQUIRED)

add_subdirectory(libseam_carving)

enable_testing()
include(GoogleTest)
add_subdirectory(tests)

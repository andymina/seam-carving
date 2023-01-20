# project setup
cmake_minimum_required(VERSION 3.4)
set(
    CMAKE_TOOLCHAIN_FILE
    "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file"
)
set(CMAKE_CXX_STANDARD 17)
project(seam-carving)

# var setup
set(INCLUDE_DIRS include/)
set(SRC_FILES src/energy.cpp)
set(TEST_DIR test/unit)
set(TARGET_NAME ${PROJECT_NAME})

# 3rd party set up
set(OPENCV_LIBS opencv_core opencv_imgcodecs opencv_highgui opencv_imgproc)
set(PYBIND11_LIBS pybind11::lto pybind11::module)
set(DOCTEST_LIBS doctest::doctest)
set(JSON_LIBS nlohmann_json::nlohmann_json)
set(GTEST_LIBS GTest::gtest_main)

# find packages
find_package(OpenCV CONFIG REQUIRED)

message(${SC_EXPORT})

# check export type
if (NOT DEFINED SC_EXPORT)
    set(SC_EXPORT "demo")
endif()

# test runner export
if (SC_EXPORT STREQUAL "test")
    if (SC_TEST_TARGET STREQUAL "")
        message(FATAL_ERROR "TEST_TARGET was not specified")
    endif ()

    message("Building ${SC_TEST_TARGET}.test.cpp")

    set(TARGET_NAME ${SC_TEST_TARGET})
    set(PROJECT_LIBS ${OPENCV_LIBS} ${GTEST_LIBS})

    find_package(GTest CONFIG REQUIRED)
    add_executable(${TARGET_NAME} ${SRC_FILES} "${TEST_DIR}/${SC_TEST_TARGET}.test.cpp")
endif ()

# test loader export
if (SC_EXPORT STREQUAL "load-tests")
    message("Populating JSON test data from test/data/*.tsv")
    set(TARGET_NAME "load-tests")

    find_package(nlohmann_json CONFIG REQUIRED)
    add_executable(${TARGET_NAME} test/utils/loader.cpp)
    set(PROJECT_LIBS ${JSON_LIBS})
endif ()

# cpp demo export
if (SC_EXPORT STREQUAL "demo")
    message("Building C++17 demo executable...")
    add_executable(${PROJECT_NAME} ${SRC_FILES} src/main.cpp)
    set(PROJECT_LIBS ${OPENCV_LIBS})
endif ()

# cpp library export
if (SC_EXPORT STREQUAL "cpp")
    message("Building C++17 static library...")
endif ()

# python package export
if (SC_EXPORT STREQUAL "py")
    message("Creating Python 3.10 package...")
    find_package(pybind11 CONFIG REQUIRED)
    pybind11_add_module(
        ${PROJECT_NAME} MODULE
        src/bind.cpp ${SRC_FILES}
    )
    set(PROJECT_LIBS ${OPENCV_LIBS} ${PYBIND11_LIBS})
    set_property(TARGET ${PROJECT_NAME} PROPERTY SUFFIX ".so")
endif ()

# js module export
if (SC_EXPORT STREQUAL "js")
    message("Creating JavaScript module...")
endif()

# target
target_include_directories(${TARGET_NAME} PRIVATE ${INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} PRIVATE ${PROJECT_LIBS})
set_target_properties(
    ${TARGET_NAME} PROPERTIES
    LIBRARY_OUTPUT_NAME "seam_carving"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/out"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/out"
)
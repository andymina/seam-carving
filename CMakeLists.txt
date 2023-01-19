# project setup
cmake_minimum_required(VERSION 3.4)
set(
    CMAKE_TOOLCHAIN_FILE
    "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file"
)
project(seam-carving)

# var setup
set(INCLUDE_DIRS include/)
set(SRC_FILES src/energy.cpp)
set(TEST_FILES test/unit/energy.test.cpp)
set(TARGET_NAME ${PROJECT_NAME})

# 3rd party set up
set(OPENCV_LIBS opencv_core opencv_imgcodecs opencv_highgui opencv_imgproc)
set(PYBIND11_LIBS pybind11::lto pybind11::module)
set(DOCTEST_LIBS doctest::doctest)
set(JSON_LIBS nlohmann_json::nlohmann_json)

# find packages
find_package(OpenCV CONFIG REQUIRED)

# check export type
if (NOT DEFINED SC_EXPORT)
    set(SC_EXPORT "demo")
endif(NOT DEFINED SC_EXPORT)

# test runner export
if (SC_EXPORT STREQUAL "tests")
    message("Building test runner...")
    find_package(doctest CONFIG REQUIRED)
    add_executable(${PROJECT_NAME} test/runner.cpp ${SRC_FILES} ${TEST_FILES})
    set(PROJECT_LIBS ${OPENCV_LIBS} ${DOCTEST_LIBS})
endif (SC_EXPORT STREQUAL "tests")

# test loader export
if (SC_EXPORT STREQUAL "load-tests")
    message("Populating JSON test data from test/data/*.tsv")
    set(TARGET_NAME "load-tests")

    find_package(nlohmann_json CONFIG REQUIRED)
    add_executable(${TARGET_NAME} test/utils/loader.cpp)
    set(PROJECT_LIBS ${JSON_LIBS})
endif (SC_EXPORT STREQUAL "load-tests")

# cpp demo export
if (SC_EXPORT STREQUAL "demo")
    message("Building C++17 demo executable...")
    add_executable(${PROJECT_NAME} src/main.cpp) # add back ${SRC_FILES}
    set(PROJECT_LIBS ${OPENCV_LIBS})
endif(SC_EXPORT STREQUAL "demo")

# cpp library export
if (SC_EXPORT STREQUAL "cpp")
    message("Building C++17 static library...")
endif(SC_EXPORT STREQUAL "cpp")

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
endif(SC_EXPORT STREQUAL "py")

# js module export
if (SC_EXPORT STREQUAL "js")
    message("Creating JavaScript module...")
endif(SC_EXPORT STREQUAL "js")

# target
target_include_directories(${TARGET_NAME} PRIVATE ${INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} PRIVATE ${PROJECT_LIBS})
set_target_properties(
    ${TARGET_NAME} PROPERTIES
    LIBRARY_OUTPUT_NAME "seam_carving"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/out"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/out"
)
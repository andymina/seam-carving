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
set(SRC_FILES src/energy.cpp src/CarvableImage.cpp)
set(OPENCV_LIBS opencv_core opencv_imgcodecs opencv_highgui)
set(PYBIND11_LIBS pybind11::lto pybind11::module)

# find packages
find_package(OpenCV CONFIG REQUIRED)

# check export type
if (NOT DEFINED SC_EXPORT)
    set(SC_EXPORT "demo")
endif(NOT DEFINED SC_EXPORT)

# cpp demo export
if (${SC_EXPORT} MATCHES "demo")
    message("Building C++17 demo executable...")
    add_executable(${PROJECT_NAME} src/main.cpp ${SRC_FILES})
    set(PROJECT_LIBS ${OPENCV_LIBS})
endif(${SC_EXPORT} MATCHES "demo")

# cpp library export
if (${SC_EXPORT} MATCHES "cpp")
    message("Building C++17 static library...")
endif(${SC_EXPORT} MATCHES "cpp")

# python package export
if (${SC_EXPORT} MATCHES "py")
    message("Creating Python 3.10 package...")
    find_package(pybind11 CONFIG REQUIRED)
    pybind11_add_module(
        ${PROJECT_NAME} MODULE
        src/bind.cpp ${SRC_FILES}
    )
    set(PROJECT_LIBS ${OPENCV_LIBS} ${PYBIND11_LIBS})
    set_property(TARGET ${PROJECT_NAME} PROPERTY SUFFIX ".so")
endif(${SC_EXPORT} MATCHES "py")

# js module export
if (${SC_EXPORT} MATCHES "js")
    message("Creating JavaScript module...")
endif(${SC_EXPORT} MATCHES "js")

# target
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIBS})
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_NAME "seam_carving"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/out"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/out"
)
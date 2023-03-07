set(
    LIB_SRCS
        energy.cpp coord.cpp seam.cpp carver.cpp)

add_library(seam_carving SHARED ${LIB_SRCS})

find_package(OpenCV CONFIG REQUIRED)

target_include_directories(
    seam_carving
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}
        PRIVATE ${CMAKE_CURRENT_LIST_DIR}/tests
)

target_link_libraries(
    seam_carving
        PRIVATE ${OPENCV_LIBS}
)

add_executable(demo)

find_package(OpenCV CONFIG REQUIRED)

target_sources(
    demo
        PRIVATE
        demo.cpp
)

target_link_libraries(
    demo
        PRIVATE seam_carving ${OPENCV_LIBS}
)

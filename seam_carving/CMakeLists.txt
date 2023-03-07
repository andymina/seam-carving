add_executable(demo)

find_package(OpenCV CONFIG REQUIRED)

target_sources(
    demo
        PRIVATE
            main.cpp
)

target_link_libraries(
    demo
        PRIVATE seam_carving
        PRIVATE ${OPENCV_LIBS}
)
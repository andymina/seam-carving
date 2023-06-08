add_executable(loader loader.cpp parser.cpp)
target_link_libraries(loader
        PRIVATE
        ${JSON_LIBS})

add_executable(generator generator.cpp)
target_link_libraries(generator
        PRIVATE
        ${OPENCV_LIBS} ${JSON_LIBS})

set(TEST_LIBS
        seam_carving
        ${OPENCV_LIBS}
        ${JSON_LIBS}
        ${GTEST_LIBS}
        CACHE INTERNAL
        "Libraries needed for testing")

add_subdirectory(unit)

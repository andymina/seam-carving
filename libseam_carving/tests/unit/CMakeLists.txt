include(${CMAKE_MODULE_DIR}/add_unit_test.cmake)

set(TEST_LIBS
    ${OPENCV_LIBS} ${JSON_LIBS} ${GTEST_LIBS}
    CACHE INTERNAL
    "Shorthand for all of the libraries needed for testing")

add_unit_test(energy "${TEST_LIBS}")


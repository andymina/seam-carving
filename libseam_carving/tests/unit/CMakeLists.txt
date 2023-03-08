include(${CMAKE_MODULE_DIR}/add_unit_test.cmake)

set(TEST_LIBS ${OPENCV_LIBS} ${JSON_LIBS} ${GTEST_LIBS})

add_unit_test(energy ${TEST_LIBS})

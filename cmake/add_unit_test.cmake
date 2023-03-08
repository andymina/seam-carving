macro(add_unit_test TESTNAME LIBRARIES)
    add_executable(${TESTNAME})
    add_test(NAME ${TESTNAME}
            COMMAND ${TESTNAME}
            WORKING_DIRECTORY ${LIBRARY_TEST_DIR})
    target_sources(${TESTNAME}
        PRIVATE
            ${TESTNAME}.test.cpp
            ${LIBRARY_TEST_DIR}/${TESTNAME}_data.cpp
    )
    target_link_libraries(${TESTNAME} PRIVATE seam_carving ${OPENCV_LIBS} ${JSON_LIBS} ${GTEST_LIBS})
    target_include_directories(${TESTNAME} PRIVATE ${LIBRARY_TEST_DIR})
endmacro()

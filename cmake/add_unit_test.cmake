macro(add_unit_test TESTNAME SOURCE_FILES)
    set(TEST_LIBS
            ${OPENCV_LIBS} ${JSON_LIBS} ${GTEST_LIBS})

    add_executable(${TESTNAME}
        ${TESTNAME}.test.cpp
        ${LIBRARY_TEST_DIR}/${TESTNAME}_data.cpp
        ${LIBRARY_TEST_DIR}/matrix_data.cpp
        ${SOURCE_FILES})
    target_link_libraries(${TESTNAME}
            PRIVATE seam_carving ${TEST_LIBS})
    target_include_directories(${TESTNAME} PRIVATE ${LIBRARY_TEST_DIR})

    gtest_discover_tests(${TESTNAME}
        EXTRA_ARGS --gtest_color=yes
        WORKING_DIRECTORY ${LIBRARY_ROOT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${LIBRARY_ROOT_DIR}")
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

macro(add_unit_test TESTNAME LIBRARIES)
    add_executable(${TESTNAME}
        ${TESTNAME}.test.cpp
        ${LIBRARY_TEST_DIR}/${TESTNAME}_data.cpp)
    target_link_libraries(${TESTNAME} PRIVATE seam_carving ${LIBRARIES})
    target_include_directories(${TESTNAME} PRIVATE ${LIBRARY_TEST_DIR})

    gtest_discover_tests(${TESTNAME}
        EXTRA_ARGS --gtest_color=yes
        WORKING_DIRECTORY ${LIBRARY_ROOT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${LIBRARY_ROOT_DIR}")
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

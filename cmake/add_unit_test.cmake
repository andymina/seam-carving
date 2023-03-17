function(add_unit_test TEST_NAME)
    set(TEST_LIBS
            ${OPENCV_LIBS} ${JSON_LIBS} ${GTEST_LIBS})

    add_executable(${TEST_NAME}
        ${TEST_NAME}.test.cpp
        ${LIBRARY_TEST_DIR}/json_utils.cpp
        ${ARGN})
    target_link_libraries(${TEST_NAME}
            PRIVATE seam_carving ${TEST_LIBS})
    target_include_directories(${TEST_NAME}
            PRIVATE ${LIBRARY_TEST_DIR})

    gtest_discover_tests(${TEST_NAME}
        EXTRA_ARGS --gtest_color=yes
        WORKING_DIRECTORY ${LIBRARY_ROOT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${LIBRARY_ROOT_DIR}")
    set_target_properties(${TEST_NAME} PROPERTIES FOLDER tests)
endfunction()

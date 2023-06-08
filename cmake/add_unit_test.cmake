function(add_unit_test TARGET_NAME TEST_PREFIX)
    add_executable(${TARGET_NAME}
            ${TEST_PREFIX}.test.cpp
            ${LIBRARY_TEST_DIR}/json_utils.cpp
            ${ARGN})
    target_link_libraries(${TARGET_NAME}
            PRIVATE ${TEST_LIBS})
    target_include_directories(${TARGET_NAME}
            PRIVATE ${LIBRARY_TEST_DIR})

    gtest_discover_tests(${TARGET_NAME}
        EXTRA_ARGS --gtest_color=no
        WORKING_DIRECTORY ${LIBRARY_ROOT_DIR})
endfunction()

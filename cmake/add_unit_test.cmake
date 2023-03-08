macro(add_unit_test TESTNAME LIBRARIES)
    add_executable(${TESTNAME})
    target_sources(${TESTNAME}
        PRIVATE
            ${LIBRARY_SOURCE_FILES}
            ${TESTNAME}.test.cpp
            ${LIBRARY_TEST_DIR}/${TESTNAME}_data.cpp
            ${LIBRARY_TEST_DIR}/test_data.cpp
    )
    target_link_libraries(${TESTNAME} PRIVATE ${LIBRARIES})
    target_include_directories(${TESTNAME} PRIVATE ${LIBRARY_ROOT_DIR} ${LIBRARY_TEST_DIR})
endmacro()

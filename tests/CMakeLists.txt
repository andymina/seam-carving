add_executable(loader loader.cpp parser.cpp)
target_link_libraries(loader
        PRIVATE
        ${JSON_LIBS})

add_executable(generator generator.cpp)
target_link_libraries(generator
        PRIVATE
        ${OPENCV_LIBS} ${JSON_LIBS})

enable_testing()
include(GoogleTest)

add_subdirectory(unit)



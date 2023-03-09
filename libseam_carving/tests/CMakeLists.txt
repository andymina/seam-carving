find_package(nlohmann_json CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

enable_testing()
include(GoogleTest)

add_subdirectory(unit)

add_executable(loader loader.cpp)
target_link_libraries(loader
    PRIVATE ${JSON_LIBS}
)

add_executable(generator generator.cpp)
target_link_libraries(generator
    PRIVATE ${OPENCV_LIBS} ${JSON_LIBS}
)

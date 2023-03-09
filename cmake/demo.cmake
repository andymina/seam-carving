add_executable(demo)

target_sources(demo PRIVATE ${PROJECT_SOURCE_DIR}/demo.cpp)
target_link_libraries(demo PRIVATE seam_carving)

set(LIBRARY_SOURCE_FILES
        ${LIBRARY_ROOT_DIR}/energy.cpp
        ${LIBRARY_ROOT_DIR}/seam.cpp
        ${LIBRARY_ROOT_DIR}/carver.cpp
        CACHE INTERNAL
        "Source files for libseam_carving")

add_library(seam_carving SHARED)
target_sources(seam_carving PRIVATE ${LIBRARY_SOURCE_FILES})
target_include_directories(seam_carving
        PUBLIC
        ${LIBRARY_ROOT_DIR})
target_link_libraries(seam_carving
        PUBLIC
        ${OPENCV_LIBS}
        ${JSON_LIBS})

#add_subdirectory(tests)

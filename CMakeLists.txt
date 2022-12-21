cmake_minimum_required(VERSION 3.1)
project(starling)

option(LIBONLY "Compile only libstarling, not starling2csv." OFF)

add_library(
    starling SHARED
    src/decode.c
    src/dissect.c
    src/sanitize.c
    src/tabulate.c
)
target_include_directories(starling PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(NOT LIBONLY)
    add_executable(starling2csv src/starling2csv.c)
    target_link_libraries(starling2csv starling)
endif()

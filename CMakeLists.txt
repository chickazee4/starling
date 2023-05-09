cmake_minimum_required(VERSION 3.1)
project(starling)

option(LIBONLY "Compile only libstarling, not starling2csv." OFF)

add_subdirectory(libstarling)

if(NOT LIBONLY)
    add_executable(starling2csv src/starling2csv.c)
    target_link_libraries(starling2csv starling)
    install(TARGETS starling2csv)
endif()
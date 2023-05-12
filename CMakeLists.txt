cmake_minimum_required(VERSION 3.12)
project(starling VERSION 0.1.9999)

option(LIBONLY "Compile only libstarling, not starling2csv." OFF)
option(STATICLIBS "Compile libstarling as static library instead of shared" OFF)

add_subdirectory(libstarling)

if(NOT LIBONLY)
    add_executable(starling2csv src/starling2csv.c)
    target_link_libraries(starling2csv starling)
    install(TARGETS starling2csv)
endif()
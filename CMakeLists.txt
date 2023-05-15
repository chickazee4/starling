cmake_minimum_required(VERSION 3.12)
cmake_policy(SET CMP0077 NEW)
project(starling VERSION 0.1.3)

option(LIBONLY "Compile only libstarling, not starling2csv." OFF)
option(STATICLIBS "Compile libstarling as static library instead of shared" OFF)

find_package(Intl REQUIRED)
include_directories(PRIVATE ${Intl_INCLUDE_DIRS})

if(NOT DEFINED CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_DATAROOTDIR /usr/share)
else()
    set(CMAKE_INSTALL_DATAROOTDIR ${CMAKE_INSTALL_PREFIX}/share)
endif()
add_definitions(-DLOCALEDIR="${CMAKE_INSTALL_DATAROOTDIR}/locale")
install(FILES locale/ru.mo DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/locale/ru_RU.utf8/LC_MESSAGES RENAME starling.mo)

add_subdirectory(libstarling)

if(NOT LIBONLY)
    add_executable(starling2csv src/starling2csv.c)
    target_link_libraries(starling2csv starling ${Intl_LIBRARY})
    install(TARGETS starling2csv)
endif()
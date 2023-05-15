if(STATICLIBS)
    add_library(
        starling STATIC
        src/decode.c
        src/dissect.c
        src/inf.c
        src/sanitize.c
        src/tabulate.c
    )
else()
    add_library(
        starling SHARED
        src/decode.c
        src/dissect.c
        src/inf.c
        src/sanitize.c
        src/tabulate.c
    )
endif()

target_include_directories(starling PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(starling ${Intl_LIBRARY})
install(TARGETS starling)
install(FILES include/starling.h DESTINATION include/starling)

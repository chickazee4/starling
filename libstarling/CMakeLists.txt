add_library(
    starling SHARED
    src/decode.c
    src/dissect.c
    src/sanitize.c
    src/tabulate.c
)
target_include_directories(starling PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
install(TARGETS starling)
install(FILES include/starling.h DESTINATION include/starling)

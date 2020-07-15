include_directories(
    "${INSTALL_EXT}/include"
    # proxygen deps include
    "${INSTALL_EXT}/deps/include"
)

add_executable(gargamelle
    gargamelle.cpp
)

target_link_libraries(gargamelle
    #    fmt
    "${INSTALL_EXT}/lib/libproxygen.a"
    "${INSTALL_EXT}/lib/libproxygenhttpserver.a"
    "${INSTALL_EXT}/deps/lib/libfolly.a"
    "${INSTALL_EXT}/deps/lib/libfmt.a"
 
)


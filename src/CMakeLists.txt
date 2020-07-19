include_directories(
    "${INSTALL_EXT}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

find_program(gperf_program gperf)
if (NOT gperf_program)
    message(FATAL_ERROR "Could not find gperf")
endif()

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/Paths.cpp"
    COMMAND ${gperf_program} --output-file=${CMAKE_CURRENT_BINARY_DIR}/Paths.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Paths.cpp.in
    MAIN_DEPENDENCY Paths.cpp.in
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    VERBATIM
)


add_executable(gargamelle
    ${CMAKE_CURRENT_BINARY_DIR}/Paths.cpp
    DumpHandler.cpp
    DumpHandler.hpp
    ErrorHandler.cpp
    ErrorHandler.hpp
    HandlerFactory.cpp
    HandlerFactory.hpp
    HelloHandler.cpp
    HelloHandler.hpp
    Paths.hpp
    gargamelle.cpp
)

# NOTE: linking order here is important, including the duplicate inclusions. In particular libproxygenhttpserver needs
# to have a libproxygen included BOTH before and after its own inclusion in order to resolve all symbols. Linking order
# was copied from the proxygen_echo sample program.
target_link_libraries(gargamelle
    ${INSTALL_EXT}/lib/libproxygen.a
    ${INSTALL_EXT}/lib/libproxygenhttpserver.a
    ${INSTALL_EXT}/lib/libproxygen.a
    ${INSTALL_EXT}/lib/libwangle.a
    ${INSTALL_EXT}/lib/libfizz.a
    ${INSTALL_EXT}/lib/libfolly.a
    ${INSTALL_EXT}/lib/libfmt.a
    z
    lzma
    lz4
    zstd
    snappy
    iberty
    sodium
    ssl
    crypto
    glog
    gflags
    double-conversion
    rt
    boost_context
    boost_filesystem
    boost_program_options
    boost_regex
    boost_system
    boost_thread
    pthread
    boost_atomic
    event
    dl
    zstd
    boost_iostreams
)

add_dependencies(gargamelle
    proxygen-install
)

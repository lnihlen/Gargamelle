include_directories(
    "${INSTALL_EXT}/include"
)

add_executable(gargamelle
    gargamelle.cpp
)

find_library(WT_LIB
    NAMES wt
    PATHS "${INSTALL_EXT}/lib"
    NO_DEFAULT_PATH
)

find_library(WTHTTP_LIB
    names wthttp
    PATHS "${INSTALL_EXT}/lib"
    NO_DEFAULT_PATH
)

target_link_libraries(gargamelle
    fmt
    spdlog
    ${WT_LIB}
    ${WTHTTP_LIB}
)

add_dependencies(gargamelle
    wt-install
)


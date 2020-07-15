include(ExternalProject)
set(INSTALL_EXT "${PROJECT_BINARY_DIR}/install-ext" CACHE PATH "Gargamelle External Dependencies")
file(MAKE_DIRECTORY "${INSTALL_EXT}")

### Wt
ExternalProject_Add(wt
    PREFIX ext
    STEP_TARGETS install
    EXCLUDE_FROM_ALL ON
    GIT_REPOSITORY https://github.com/emweb/wt
    GIT_TAG master
    GIT_PROGRESS ON
    UPDATE_COMMAND ""
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${INSTALL_EXT}
)

### fmt
add_subdirectory(fmt)

### spdlog
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "require spdlog to use external fmt")
add_subdirectory(spdlog)


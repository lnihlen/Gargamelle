include(ExternalProject)
set(INSTALL_EXT "${PROJECT_BINARY_DIR}/install-ext" CACHE PATH "Gargamelle External Dependencies")
file(MAKE_DIRECTORY "${INSTALL_EXT}")

### fmt
ExternalProject_Add(fmt
    PREFIX ext
    STEP_TARGETS install
    EXCLUDE_FROM_ALL ON
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 6.2.1
    UPDATE_COMMAND ""
    CMAKE_ARGS -DCMAKE_PREFIX_PATH=${INSTALL_EXT}
               -DCMAKE_INSTALL_PREFIX=${INSTALL_EXT}
               -DCMAKE_BUILD_TYPE=RelWithDebInfo
               -DFMT_DOC=OFF
               -DFMT_TEST=OFF
)

### folly
ExternalProject_Add(folly
    PREFIX ext
    DEPENDS fmt-install
    STEP_TARGETS install
    EXCLUDE_FROM_ALL ON
    GIT_REPOSITORY https://github.com/facebook/folly.git
    # source hash from proxygen/build/deps/github_hashes/facebook/folly-rev.txt
    GIT_TAG e4df98ab10400b02efef57cc9cb38ceba2156200
    UPDATE_COMMAND ""
    CMAKE_ARGS -DCMAKE_PREFIX_PATH=${INSTALL_EXT}
               -DCMAKE_INSTALL_PREFIX=${INSTALL_EXT}
               -DCMAKE_BUILD_TYPE=RelWithDebInfo
               -DBUILD_TESTS=OFF
               -DBUILD_SHARED_LIBS=OFF
)

### fizz
ExternalProject_Add(fizz
    PREFIX ext
    DEPENDS folly-install
    STEP_TARGETS install
    EXCLUDE_FROM_ALL ON
    GIT_REPOSITORY https://github.com/facebookincubator/fizz
    # source hash from proxygen/build/deps/github_hashes/facebookincubator/fizz-rev.txt
    GIT_TAG 8798f7067fad5c7467bdeb1c29efd899658df6ff
    UPDATE_COMMAND ""
    SOURCE_SUBDIR fizz
    CMAKE_ARGS -DCMAKE_PREFIX_PATH=${INSTALL_EXT}
               -DCMAKE_INSTALL_PREFIX=${INSTALL_EXT}
               -DCMAKE_BUILD_TYPE=RelWithDebInfo
               -DBUILD_TESTS=OFF
               -DBUILD_SHARED_LIBS=OFF
)

### wangle
ExternalProject_Add(wangle
    PREFIX ext
    DEPENDS fizz-install
    STEP_TARGETS install
    EXCLUDE_FROM_ALL ON
    GIT_REPOSITORY https://github.com/facebook/wangle
    # source hash from proxygen/build/deps/github_hashes/facebook/wangle-rev.txt
    GIT_TAG 4dadae28eaca99846ff30413fdb008d34acda007
    UPDATE_COMMAND ""
    SOURCE_SUBDIR wangle
    CMAKE_ARGS -DCMAKE_PREFIX_PATH=${INSTALL_EXT}
               -DCMAKE_INSTALL_PREFIX=${INSTALL_EXT}
               -DCMAKE_BUILD_TYPE=RelWithDebInfo
               -DBUILD_TESTS=OFF
               -DBUILD_SHARED_LIBS=OFF
)

### proxygen
ExternalProject_Add(proxygen
    PREFIX ext
    DEPENDS wangle-install
    STEP_TARGETS install
    EXCLUDE_FROM_ALL ON
    GIT_REPOSITORY https://github.com/facebook/proxygen
    GIT_TAG v2020.07.13.00
    UPDATE_COMMAND ""
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=RelWithDebInfo
               -DCMAKE_PREFIX_PATH=${INSTALL_EXT}
               -DCMAKE_INSTALL_PREFIX=${INSTALL_EXT}
               -DBUILD_QUIC=OFF
               -DBUILD_TESTS=OFF
               -DBUILD_FUZZERS=OFF
               -DBUILD_SHARED_LIBS=OFF
)


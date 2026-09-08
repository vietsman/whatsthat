cmake_minimum_required(VERSION 3.16)
project(whatsthat VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

include(GNUInstallDirs)

find_package(PkgConfig REQUIRED)

set(WHATSTHAT_DEPS
    libsodium
    protobuf-c
    libwebsockets
    libcurl
    cjson
    sqlite3
    zlib
)

foreach(dep IN LISTS WHATSTHAT_DEPS)
    pkg_check_modules(PC_${dep} QUIET ${dep})
    if(PC_${dep}_FOUND)
        message(STATUS "Found optional dependency via pkg-config: ${dep}")
    else()
        message(STATUS "Optional dependency not found yet: ${dep}")
    endif()
endforeach()

add_library(whatsthat
    src/api/client.c
    src/crypto/crypto_stub.c
    src/http/http_stub.c
    src/net/net_stub.c
    src/proto/proto_stub.c
    src/store/store_stub.c
    src/util/util_stub.c
    src/ws/ws_stub.c
)

target_include_directories(whatsthat
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_options(whatsthat PRIVATE -Wall -Wextra -Wpedantic)

add_executable(whatsthat_cli examples/cli/main.c)
target_link_libraries(whatsthat_cli PRIVATE whatsthat)

install(TARGETS whatsthat
    EXPORT whatsthatTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_file(cmake/whatsthat.pc.in whatsthat.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/whatsthat.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

enable_testing()
add_test(NAME smoke_cli COMMAND whatsthat_cli --version)

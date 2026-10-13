cmake_minimum_required(VERSION 3.16)
project(whatsthat VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

include(GNUInstallDirs)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PROTOBUF_C QUIET IMPORTED_TARGET libprotobuf-c)
if(NOT PROTOBUF_C_FOUND)
    pkg_check_modules(PROTOBUF_C REQUIRED IMPORTED_TARGET protobuf-c)
endif()

option(WHATSTHAT_REGENERATE_PROTO "Regenerate protobuf-c files during the build if protoc is available" OFF)

find_program(PROTOC_EXECUTABLE protoc)
find_program(PROTOC_GEN_C_EXECUTABLE protoc-gen-c)

file(GLOB_RECURSE WHATSTHAT_PROTO_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/proto/generated/*.pb-c.c"
)
file(GLOB_RECURSE WHATSTHAT_PROTO_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/proto/generated/*.pb-c.h"
)
file(GLOB_RECURSE WHATSTHAT_PROTO_INPUTS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/third_party/protos/proto" CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/protos/proto/*.proto"
)

set(WHATSTHAT_DEPS
    libsodium
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
    src/proto/helpers.c
    src/store/store_stub.c
    src/util/util_stub.c
    src/ws/ws_stub.c
    ${WHATSTHAT_PROTO_SOURCES}
)

target_include_directories(whatsthat
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/proto/generated
)

target_compile_options(whatsthat PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(whatsthat PUBLIC PkgConfig::PROTOBUF_C)

if(PROTOC_EXECUTABLE AND PROTOC_GEN_C_EXECUTABLE)
    add_custom_target(whatsthat_proto_generate
        COMMAND ${PROTOC_EXECUTABLE}
            --proto_path=${CMAKE_CURRENT_SOURCE_DIR}/third_party/protos/proto
            --plugin=protoc-gen-c=${PROTOC_GEN_C_EXECUTABLE}
            --c_out=${CMAKE_CURRENT_SOURCE_DIR}/src/proto/generated
            ${WHATSTHAT_PROTO_INPUTS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/third_party/protos/proto
        COMMENT "Regenerating protobuf-c bindings from third_party/protos/proto"
        VERBATIM
    )
    if(WHATSTHAT_REGENERATE_PROTO)
        add_dependencies(whatsthat whatsthat_proto_generate)
    endif()
elseif(WHATSTHAT_REGENERATE_PROTO)
    message(FATAL_ERROR "WHATSTHAT_REGENERATE_PROTO=ON requires both protoc and protoc-gen-c in PATH")
endif()

add_executable(whatsthat_cli examples/cli/main.c)
target_link_libraries(whatsthat_cli PRIVATE whatsthat)

add_executable(whatsthat_proto_roundtrip tests/proto_roundtrip.c)
target_include_directories(whatsthat_proto_roundtrip PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/proto/generated
)
target_link_libraries(whatsthat_proto_roundtrip PRIVATE whatsthat)

install(TARGETS whatsthat
    EXPORT whatsthatTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY src/proto/generated/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/whatsthat/proto/generated
    FILES_MATCHING PATTERN "*.pb-c.h"
)

configure_file(cmake/whatsthat.pc.in whatsthat.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/whatsthat.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

enable_testing()
add_test(NAME smoke_cli COMMAND whatsthat_cli --version)
add_test(NAME proto_roundtrip COMMAND whatsthat_proto_roundtrip)

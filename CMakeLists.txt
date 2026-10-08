cmake_minimum_required(VERSION 3.28)

project(vmnl_net
  VERSION 0.1.0
  DESCRIPTION "Networking library in C"
  HOMEPAGE_URL "https://github.com/VMNL/vmnl-net"
  LANGUAGES C
)

include(GenerateExportHeader)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(BUILD_SHARED_LIBS "Build a shared library instead of a static one" OFF)
option(VMNL_NET_BUILD_TESTS "Build the tests" ${PROJECT_IS_TOP_LEVEL})
option(VMNL_NET_BUILD_DOCS "Generate the documentation with Doxygen" OFF)
option(VMNL_NET_INSTALL "Generate the install rules" ${PROJECT_IS_TOP_LEVEL})

if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    endif()
endif()

add_library(vmnl_net_options INTERFACE)
add_library(vmnl_net_warnings INTERFACE)

add_library(vmnl_net)
add_library(vmnl::net ALIAS vmnl_net)

generate_export_header(vmnl_net
  EXPORT_MACRO_NAME VMNL_NET_API
  EXPORT_FILE_NAME include/vmnl/net_export.h
)

target_sources(vmnl_net
  PRIVATE
    src/core/vmnl_net_version.c
    src/core/vmnl_net_version_string.c
    src/error/vmnl_net_error_string.c
    src/context/vmnl_net_context_create.c
    src/context/vmnl_net_context_create_with_allocator.c
    src/context/vmnl_net_context_destroy.c
    $<$<NOT:$<PLATFORM_ID:Windows>>:src/clock/posix/vmnl_net_clock_now.c>
    $<$<PLATFORM_ID:Windows>:src/clock/win32/vmnl_net_clock_now.c>
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include ${PROJECT_BINARY_DIR}/include
    FILES
      ${PROJECT_BINARY_DIR}/include/vmnl/net_export.h
      include/vmnl/net.h
      include/vmnl/net/core.h
      include/vmnl/net/error.h
      include/vmnl/net/context.h
      include/vmnl/net/clock.h
)

set_target_properties(vmnl_net_options PROPERTIES INTERFACE_POSITION_INDEPENDENT_CODE ON)

if(NOT MSVC)
    target_compile_options(vmnl_net_warnings
      INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
        -Wsign-conversion
        -Wcast-align
        -Wpointer-arith
        -Wvla
        -Wformat=2
        $<$<COMPILE_LANGUAGE:C>:-Wstrict-prototypes>
        $<$<COMPILE_LANGUAGE:C>:-Wmissing-prototypes>
    )
else()
    target_compile_options(vmnl_net_warnings INTERFACE /W4)
endif()

target_include_directories(vmnl_net PRIVATE src)

target_compile_features(vmnl_net PUBLIC c_std_11)

target_compile_definitions(vmnl_net
  PRIVATE
    VMNL_NET_VERSION="${PROJECT_VERSION}"
    VMNL_NET_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    VMNL_NET_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    VMNL_NET_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    $<$<NOT:$<PLATFORM_ID:Windows>>:_POSIX_C_SOURCE=200809L>
    $<$<PLATFORM_ID:Windows>:WIN32_LEAN_AND_MEAN>
)

target_link_libraries(vmnl_net
  PRIVATE
    $<BUILD_INTERFACE:vmnl_net_options>
    $<BUILD_INTERFACE:vmnl_net_warnings>
)

set_target_properties(vmnl_net
  PROPERTIES
    C_VISIBILITY_PRESET hidden
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(VMNL_NET_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(VMNL_NET_BUILD_DOCS)
    include(cmake/doxygen.cmake)
endif()

if(VMNL_NET_INSTALL)
    include(cmake/install.cmake)
endif()

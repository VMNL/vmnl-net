cmake_minimum_required(VERSION 3.28)

project(vmnl_net
  VERSION 0.2.0
  DESCRIPTION "Networking library in C"
  HOMEPAGE_URL "https://github.com/VMNL/vmnl-net"
  LANGUAGES C
)

include(GenerateExportHeader)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(BUILD_SHARED_LIBS "Build a shared library instead of a static one" OFF)
option(VMNL_NET_BUILD_DOCS "Generate the documentation with Doxygen" OFF)
option(VMNL_NET_BUILD_TESTS "Build the tests" ${PROJECT_IS_TOP_LEVEL})
option(VMNL_NET_BUILD_BENCHMARKS "Build the benchmarks" OFF)
option(VMNL_NET_BUILD_EXAMPLES "Build the examples" ${PROJECT_IS_TOP_LEVEL})
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
    EXPORT_FILE_NAME include/vmnl/net_api.h
)

target_sources(vmnl_net
    PRIVATE
        src/version/version.c
        src/version/version_string.c
        src/error/error_as_string.c
        src/allocator/allocator_set.c
        src/context/context_create.c
        src/context/context_destroy.c
        src/clock/clock_elapsed.c
        $<$<NOT:$<PLATFORM_ID:Windows>>:src/clock/posix/clock_now.c>
        $<$<PLATFORM_ID:Windows>:src/clock/win32/clock_now.c>
        # src/timer/timer_create.c
        # src/timer/timer_destroy.c
        # src/timer/timer_remaining.c
        # src/timer/timer_start.c
        # src/timer/timer_stop.c
        # src/timer/timer_update.c
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include ${PROJECT_BINARY_DIR}/include
        FILES
            ${PROJECT_BINARY_DIR}/include/vmnl/net_api.h
            include/vmnl/net/attribute.h
            include/vmnl/net/context.h
            include/vmnl/net/version.h
            include/vmnl/net/error.h
            include/vmnl/net/allocator.h
            include/vmnl/net/clock.h
            # include/vmnl/net/timer.h
)

set_target_properties(vmnl_net_options PROPERTIES INTERFACE_POSITION_INDEPENDENT_CODE ON)

if(NOT MSVC)
    target_compile_options(vmnl_net_warnings
      INTERFACE
        -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wcast-align -Wpointer-arith -Wvla -Wformat=2
        $<$<COMPILE_LANGUAGE:C>:-Wstrict-prototypes -Wmissing-prototypes>
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

if(PROJECT_IS_TOP_LEVEL)
    include(${PROJECT_SOURCE_DIR}/cmake/compile_commands.cmake)
    include(${PROJECT_SOURCE_DIR}/cmake/format.cmake)
endif()

if(VMNL_NET_BUILD_DOCS)
    include(${PROJECT_SOURCE_DIR}/cmake/doxygen.cmake)
endif()

if(VMNL_NET_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
    include(${PROJECT_SOURCE_DIR}/cmake/coverage.cmake)
endif()

if(VMNL_NET_BUILD_BENCHMARKS)
    add_subdirectory(bench)
endif()

# if(VMNL_NET_BUILD_EXAMPLES)
    add_subdirectory(examples)
# endif()

if(VMNL_NET_INSTALL)
    include(${PROJECT_SOURCE_DIR}/cmake/install.cmake)
endif()

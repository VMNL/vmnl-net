cmake_minimum_required(VERSION 3.28..4.2)
project(vmnl-network
  VERSION 0.1
  DESCRIPTION "Sockets wrapper & additional features for VMNL library"
  LANGUAGES C
)

# set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(BUILD_SHARED_LIBS TRUE)

add_library(${PROJECT_NAME})

target_compile_options(${PROJECT_NAME}
  PRIVATE
    $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
      -Wall -Wextra -Wpedantic -Wshadow -O3 -D_FORTIFY_SOURCE=3>
    $<$<C_COMPILER_ID:MSVC>:
      /Wall /W4 /D_FORTIFY_SOURCE=3>
)

set_target_properties(${PROJECT_NAME}
  PROPERTIES
    C_STANDARD 11
    C_EXTENSIONS FALSE
    C_STANDARD_REQUIRED TRUE

    # for Winslop, a RUNTIME is a Dll
    RUNTIME_OUTPUT_DIRECTORY "$<1:${CMAKE_BINARY_DIR}/lib>"
    LIBRARY_OUTPUT_DIRECTORY "$<1:${CMAKE_BINARY_DIR}/lib>"
    ARCHIVE_OUTPUT_DIRECTORY "$<1:${CMAKE_BINARY_DIR}/lib>"
    BUILD_RPATH "$<1:${CMAKE_BINARY_DIR}/lib>"
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

target_sources(${PROJECT_NAME}
  PRIVATE
    src/socket/basic_socket.c
    src/buffer/buffer.c
    src/buffer/dynamic_buffer.c
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32)
endif()

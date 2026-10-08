enable_language(CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
include(GoogleTest)

set(INSTALL_GTEST OFF)
set(gtest_force_shared_crt ON)

FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.18.0.tar.gz
  URL_HASH SHA256=6e3191c1455468b3fc35a417fb565c1c5071aee1b7e7f85e30cf48a98d37d8b5
  SYSTEM
  FIND_PACKAGE_ARGS NAMES GTest
)

block()
    set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)
    FetchContent_MakeAvailable(googletest)
endblock()

add_executable(vmnl_net_tests)

target_sources(vmnl_net_tests
  PRIVATE
    core/vmnl_net_version_tests.cpp
    core/vmnl_net_version_string_tests.cpp
    error/vmnl_net_error_string_tests.cpp
    context/vmnl_net_context_create_tests.cpp
    context/vmnl_net_context_create_with_allocator_tests.cpp
    context/vmnl_net_context_destroy_tests.cpp
    clock/vmnl_net_clock_now_tests.cpp
)

target_link_libraries(vmnl_net_tests
  PRIVATE
    vmnl::net
    vmnl_net_warnings
    GTest::gtest_main
)

gtest_discover_tests(vmnl_net_tests DISCOVERY_MODE PRE_TEST)

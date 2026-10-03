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

FetchContent_MakeAvailable(googletest)

add_executable(vmnl_net_tests)

target_sources(vmnl_net_tests
  PRIVATE
    version/vmnl_net_version_tests.cpp
    version/vmnl_net_version_string_tests.cpp
)

target_link_libraries(vmnl_net_tests
  PRIVATE
    vmnl::net
    vmnl_net_warnings
    GTest::gtest_main
)

gtest_discover_tests(vmnl_net_tests DISCOVERY_MODE PRE_TEST)

# Build

Several presets of `CMakePresets.json` build and check the library, each run from the root of the repository with `cmake --workflow --preset <preset>`, and built into `build/<preset>`.

## Requirements

- CMake 3.28 or later and one of the compilers listed in the README
- a C++17 compiler for the benchmarks
- clang-format 22 for the formatting
- GCC and gcovr 8.6 for the coverage
- Doxygen 1.18 for the documentation

## Presets

| Preset | Use | Platforms |
| --- | --- | --- |
| `release` | Builds the library in Release | All |
| `debug` | Builds the library in Debug | All |
| `test` | Builds and runs the tests in Debug | All |
| `sanitize` | Runs the tests under AddressSanitizer and UndefinedBehaviorSanitizer | Linux, macOS |
| `coverage` | Measures the coverage of the tests, which must stay at 100% of lines and branches | Linux, macOS |
| `bench` | Builds the benchmarks in Release, run with `./build/bench/bin/vmnl_net_benchmarks` | All |
| `docs` | Generates the documentation in `build/docs/docs/html`, and fails on any warning | All |
| `format` | Formats the sources with clang-format | All |
| `format-check` | Checks the formatting, like the CI | All |

`cmake --list-presets=all` lists them with the names shown in the IDEs.

To add the internal headers to the documentation, configure the `docs` preset with `-DVMNL_NET_BUILD_DOCS_INTERNAL=ON`, then build it with `cmake --build --preset docs`.

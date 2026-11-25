# Build

Several presets of `CMakePresets.json` build and check the library, each run from the root of the repository with `cmake --workflow --preset <preset>`, and built into `build/<preset>`.

## Requirements

- CMake 3.28 or later and one of the compilers listed in the README
- a C++17 compiler for the benchmarks
- clang-format 23.1.4 for the formatting
- clang-tidy 23.1.4 and Ninja for the static analysis
- gcovr 8.6, with GCC or Clang, for the coverage
- Mull 0.39.1 and the Clang of LLVM 19, on Linux or macOS, for the mutation testing
- Doxygen 1.18 for the documentation

## Presets

| Preset | Use | Platforms |
| --- | --- | --- |
| `release` | Builds the library in Release | All |
| `debug` | Builds the library in Debug | All |
| `test` | Builds and runs the tests in Release, like the CI | All |
| `sanitize-address` | Runs the tests under AddressSanitizer and UndefinedBehaviorSanitizer, in RelWithDebInfo | Linux, macOS |
| `sanitize-memory` | Runs the tests under MemorySanitizer, built with Clang, in RelWithDebInfo | Linux |
| `coverage` | Measures the coverage of the tests in Debug, which must stay at 100% of lines and branches, with its report in `build/coverage/report/html` | Linux, macOS |
| `mutation` | Builds the tests in Debug with the Clang given by `CC` and Mull's plugin given by `MULL_PLUGIN`, then runs the mutants, with its report in `build/mutation/report/html` | Linux, macOS |
| `bench` | Builds the benchmarks in Release, run with `./build/bench/bin/vmnl_net_benchmarks` | All |
| `docs` | Generates the documentation in `build/docs/docs/html`, and fails on any warning | All |
| `docs-internal` | Generates the documentation with the internal headers, in `build/docs-internal/docs/html` | All |
| `format` | Formats the sources with clang-format | All |
| `format-check` | Checks the formatting, like the CI | All |
| `tidy` | Builds the library with Ninja and checks it with clang-tidy, like the CI | All |

`cmake --list-presets=all` lists them with the names shown in the IDEs.

The presets read the environment variable `VMNL_NET_WARNINGS_AS_ERRORS` into the option of the same name, which turns the warnings of the library, the tests, the benchmarks and the examples into errors. The CI sets it to `ON`, so a warning fails its build. Set it to reproduce a CI failure locally, as in `VMNL_NET_WARNINGS_AS_ERRORS=ON cmake --workflow --preset test`.

## When something breaks

The library runs no service, so there are no logs to read and nothing to restart.
A failing function reports an error code, and `vmnl_net_error_as_string()` gives its message.

- A CI check fails. Run the preset of its job with `VMNL_NET_WARNINGS_AS_ERRORS=ON`, as above.
- The documentation fails on a link to a decision record. Doxygen leaves `docs/decisions` out, so link the record through its GitHub URL.
- clang-tidy misses the system headers on macOS. The `tidy` preset finds the SDK with `xcrun --show-sdk-path`, which needs the Xcode command line tools.
- The Benchmarks job fails. A benchmark skipped with an error, and the job log shows its message after `ERROR OCCURRED`.
- The Mutation job fails. A mutant survives on Linux and macOS, and the job summary lists it. Catch it with a test, or simplify the code when the mutant shows a part that serves nothing.
- A function reports `VMNL_NET_ENOMEM` or `VMNL_NET_ENOBUFS`. The [points of failure](architecture.md#points-of-failure) of the architecture say what the library does then.

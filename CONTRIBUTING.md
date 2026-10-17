# Contributing

This page lists the checks to run before opening a pull request and the few conventions the tools do not enforce.

## Checks

You need CMake 3.28 or later, clang-format 22, Doxygen and gcovr, and a C++17 compiler for the benchmarks.
Each check is a workflow of `CMakePresets.json`, run from the root of the repository, that builds into `build/<preset>`:

```sh
# Every preset, with what it does
cmake --list-presets=all

# Formatting, fixed with --preset format
cmake --workflow --preset format-check

# Library only, in Release
cmake --workflow --preset lib

# Build and tests, in Debug
cmake --workflow --preset dev

# Build and tests, in Release like the CI, with release-shared for the shared library
cmake --workflow --preset release-static

# Sanitizers, on Linux and macOS
cmake --workflow --preset sanitize

# Coverage, which must stay at 100% of lines and branches with GCC and gcovr 8.6 like the CI, outside Windows
cmake --workflow --preset coverage

# Benchmarks, built in Release for meaningful numbers
cmake --workflow --preset bench
./build/bench/bin/vmnl_net_benchmarks

# Documentation, which fails on any warning
cmake --workflow --preset docs
```

A change to the public API comes with its Doxygen comments and tests, a new module with an example in `examples/example_<module>_<feature>.c`, a bug fix with a regression test, and a new feature with its benchmarks and an update of the roadmap in the README.

## Conventions

clang-format takes care of the layout. On top of it:

- names follow the [rules of VMNL](https://github.com/VMNL/vmnl/blob/main/docs/INSTRUCTIONS.md): `VmnlNet` types, `vmnl_net_` functions, `VMNL_NET_` macros, `is_` for predicates and `as_` for conversions;
- each public function has its own file, named after it without the `vmnl_net_` prefix, like `src/buffer/buffer_create_flat.c`, private code goes in `src/<module>/<module>_internal.h`, an internal module used by several others, like the list, in its own folder, and platform code in `posix/` or `win32/`;
- includes go from the public headers of the library to the private ones, then the dependencies and the C library, one block each, and a file only includes what it needs;
- a function that can fail takes a nullable `VmnlNetError *error` as last parameter, sets it on every call, and tells the failure through its return value;
- like in the libc, a pointer parameter is not checked against NULL unless it is nullable, except the functions given by the caller, like an allocator, which give `VMNL_NET_EINVAL`;
- `sizeof` takes no parentheses on an expression, compound literals take a space after the type, counters use `i++` rather than `++i`, and booleans are compared with `== true` or `== false` rather than `!`;
- Doxygen comments stay short and technical, with `nullable` for optional pointers, the error codes listed under the `error` parameter, the contracts in `\attention` with `**MUST**` and a short reason, the other facts in `\remark`, and snippets that compile.

## Tests

Tests use [utest.h](https://github.com/sheredom/utest.h), fetched by CMake.
Each public function has its own test file, named like its source, `tests/<module>/<name>_tests.c`, and listed in `tests/CMakeLists.txt`.
The suite takes the name of the function, like `vmnl_net_buffer_create_flat`, and each test the case it covers in snake case, like `success` or `null_error`, or a short sentence for a property, like `never_goes_backwards`.
A `VmnlNetError` starts at `UINT32_MAX`, so the test proves the function writes it.
Tests are ordered from `success` to the properties, the limits like `zero_capacity`, the allocation failures, the `null_` cases of the nullable parameters, and `null_error` last.

```c
UTEST(vmnl_net_buffer_create_flat, success)
{
    const size_t capacity = 64;
    VmnlNetError error = UINT32_MAX;
    VmnlNetBuffer *buffer = vmnl_net_buffer_create_flat(capacity, &error);

    ASSERT_TRUE(buffer != NULL);
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_EQ(vmnl_net_buffer_capacity(buffer), capacity);
    vmnl_net_buffer_destroy(buffer);
}
```

CTest runs each test in its own process, so a test that replaces the allocator cannot break the next ones, but it still restores the default one before it returns.

## Benchmarks

Benchmarks use [Google Benchmark](https://github.com/google/benchmark), fetched by CMake when `VMNL_NET_BUILD_BENCHMARKS` is `ON`, and they are the only C++ code of the repository.
Each public function has its own benchmark file, `bench/<module>/<name>_benchmarks.cpp`, listed in `bench/CMakeLists.txt`.
A benchmark takes the name of the function without the `vmnl_net_` prefix, which would clash with the function itself, followed by the case when there are several, like `buffer_prepare_ring`, and covers its sizes with `Range` or `Args` rather than one benchmark per size.
To measure a change, compare the runs before and after it with the `compare.py` tool of Google Benchmark.

## Commits and Pull Requests

Commit messages follow [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/): `<type>[optional scope]: <description>`, in the imperative, starting with a lowercase letter and without a final period.
The subject stays under 72 characters and the lines of the body under 80.
The types are `build`, `cicd`, `chore`, `docs`, `feat`, `fix`, `perf`, `refactor`, `style` and `test`, described in the [instructions of VMNL](https://github.com/VMNL/vmnl/blob/main/docs/INSTRUCTIONS.md#commit-type-usage-guidelines).
Before `1.0.0`, any version may break the API, so breaking changes are not marked with `!`.

Install the hook that checks every message before the commit:

```sh
git config core.hooksPath .githooks
```

A pull request has a title in the format of a commit and a description of a few plain sentences, since it becomes the body of the merge commit.
It closes the issue of its feature with `Closes #N`, and the issue carries the milestone of the version and the labels.

## Markdown

Markdown files are written in English, one sentence per line, with a single blank line before each heading.

# Contributing

This page lists the checks to run before opening a pull request and the few conventions the tools do not enforce.

## Checks

You need CMake, clang-format 22, Doxygen and gcovr.
Run these commands from the root of the repository:

```sh
# Formatting, fixed by replacing --dry-run --Werror with -i
find src include tests \( -name '*.[ch]' -o -name '*.cpp' \) -exec clang-format --dry-run --Werror {} +

# Build and tests
cmake -B build
cmake --build build
ctest --test-dir build --output-on-failure

# Sanitizers
CFLAGS=-fsanitize=address,undefined CXXFLAGS=-fsanitize=address,undefined LDFLAGS=-fsanitize=address,undefined cmake -B build-sanitize -DCMAKE_BUILD_TYPE=Debug
cmake --build build-sanitize
ctest --test-dir build-sanitize --output-on-failure

# Coverage, which must stay at 100% of lines and branches
CFLAGS=--coverage LDFLAGS=--coverage cmake -B build-coverage -DCMAKE_BUILD_TYPE=Debug
cmake --build build-coverage
ctest --test-dir build-coverage
gcovr --root . --filter src/ --fail-under-line 100 --fail-under-branch 100 build-coverage

# Documentation, which fails on any warning
cmake -B build -DVMNL_NET_BUILD_DOCS=ON
cmake --build build --target docs
```

With Apple Clang, gcovr also needs `--gcov-executable "xcrun llvm-cov gcov"`.

A change to the public API comes with its Doxygen comments and tests, a bug fix with a regression test, and a new feature with an update of the roadmap in the README.

## Conventions

clang-format takes care of the layout. On top of it:

- each public function has its own file, `src/<module>/<function>.c`, private code goes in `src/<module>/<module>_internal.h` and platform code in `posix/` or `win32/`;
- includes go from the public headers of the library to the private ones, then the dependencies and the C library, one block each, and a file only includes what it needs;
- a function that can fail takes a nullable `VmnlNetError *error` as last parameter, sets it on every call, and tells the failure through its return value;
- `sizeof` takes no parentheses on an expression, compound literals take a space after the type, and counters use `i++` rather than `++i`;
- Doxygen comments stay short and factual, with `nullable` for optional pointers, the error codes listed under the `error` parameter, and `**MUST**` for what the caller has to do.

## Tests

Each public function has its own GoogleTest file, `tests/<module>/<function>_tests.cpp`.
The suite takes the name of the function, like `VmnlNetContextCreate`, and each test the case it covers, like `Success` or `NullError`, or a short sentence for a property, like `NeverGoesBackwards`.
A `VmnlNetError` starts at `UINT32_MAX`, so the test proves the function writes it.

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

# Contributing

This page lists the checks to run before opening a pull request and the few conventions the tools do not enforce.

## Workflow

The code must be clean and efficient, with the data structures and the algorithms that fit each problem.
Several presets of `CMakePresets.json` check it, each run from the root of the repository with `cmake --workflow --preset <preset>`, and built into `build/<preset>`.

### Requirements

- CMake 3.28 or later and one of the compilers listed in the README
- a C++17 compiler for the benchmarks
- clang-format 22 for the formatting
- GCC and gcovr 8.6 for the coverage
- Doxygen 1.18 for the documentation

### Presets

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

A change to the public API comes with its Doxygen comments and tests, a new module with an example in `examples/example_<module>_<feature>.c`, a bug fix with a regression test, and a new feature with its benchmarks and an update of the roadmap in the README.

## Conventions

clang-format takes care of the layout, the rules below cover the rest.

### Names

They follow the [rules of VMNL](https://github.com/VMNL/vmnl/blob/main/docs/INSTRUCTIONS.md).

| Element | Form | Example |
| --- | --- | --- |
| Type | `VmnlNet<Name>` | `VmnlNetBuffer` |
| Function | `vmnl_net_<module>_<action>` | `vmnl_net_buffer_create` |
| Macro | `VMNL_NET_<NAME>` | `VMNL_NET_EINVAL` |
| Predicate | `vmnl_net_<module>_is_<state>` | `vmnl_net_buffer_is_empty` |
| Conversion | `vmnl_net_<module>_as_<type>` | `vmnl_net_error_as_string` |

### Files

- one public function per file, named without the `vmnl_net_` prefix: `src/buffer/buffer_create.c`
- the operations behind a public function in one folder per type or family: an operation shared by a whole family takes its name, like `src/buffer/contiguous/buffer_contiguous_clear.c`, and one shared by some types keeps the name of the type that defines it, the others pointing to it or calling it, like the dynamic buffer committing with `vmnl_net_buffer_flat_commit()`
- private code in `src/<module>/<module>_internal.h`
- an internal module used by several others in its own folder, like `src/list/`
- platform code in `posix/` or `win32/`
- includes in four blocks: public headers of the library, private ones, dependencies, C library
- a file includes only what it needs

### Errors and NULL

- a function that can fail takes a nullable `VmnlNetError *error` last, sets it on every call, and tells the failure through its return value
- NULL is accepted only where it means something: an `error` the caller does not read, a `user_data` it does not need, or nothing to destroy
- any other pointer is used without check, like `strlen(NULL)` in the libc, so a NULL one is a bug of the caller

### Not checked by clang-format

- `sizeof` without parentheses on an expression: `sizeof *context`
- counters with `it++` and `it--`, not `++it` and `--it`, unless the expression needs the updated value
- comparisons written out for readability: booleans with `== true` or `== false`, pointers with `== NULL` or `!= NULL`, not `!value` nor `value` alone
- the condition of a ternary in parentheses: `(now > start) ? now - start : 0`

### Doxygen

- short and technical, with snippets that compile
- in the public headers, what the caller needs, said once, without implementation detail
- in the internal headers, the implementation details the next developer needs too
- `nullable` for optional pointers
- the error codes listed under the `error` parameter
- the contracts in `\attention`, with `**MUST**` and a short reason
- the other facts in `\remark`

## Tests

Tests use [utest.h](https://github.com/sheredom/utest.h), fetched by CMake, and follow the formatting of the library.

- one file per public function, named like its source, for the behavior every type shares: `tests/buffer/buffer_prepare_tests.c`
- one file per type and public function, in a folder per type, for the behavior of that type only: `tests/buffer/ring/buffer_ring_prepare_tests.c`
- the files listed in `tests/CMakeLists.txt`, and the helpers of a module in `tests/<module>/<module>_tests.h`
- the C library comes from `utest.h`, which the tests do not include again
- the generic containers, like the list and later a hash map or a dynamic array, are tested directly through their internal header, one file per function or macro: `tests/list/list_splice_tests.c`; the other internal functions are covered through the public ones
- the suite is the name of the function or the macro, with the type for the behavior of one type: `vmnl_net_buffer_prepare`, `vmnl_net_buffer_ring_prepare`, `VMNL_NET_LIST_FOREACH`
- the behavior every type shares runs once per type with `UTEST_I`, its index following the `types` of `tests/buffer/buffer_tests.h`: `vmnl_net_buffer_prepare.keeps_stored_bytes/1` is the ring buffer
- each test says what the function does, `<verb>_<object>_<condition>`, in snake case, the verb first, without article, `vmnl_net` nor the type of the suite, the condition optional and starting with `when`, `with`, `without` or `after`: `returns_null_when_empty`, `prepares_two_views_when_free_space_wraps`
- the recurring cases keep the same name:

| Case | Name |
| --- | --- |
| Value returned | `returns_<value>`: `returns_zero_when_empty` |
| Property kept | `keeps_<property>`: `keeps_stored_bytes` |
| Documented no-op | `ignores_<argument>`: `ignores_zero_bytes` |
| `VMNL_NET_EINVAL` | `rejects_<argument>`: `rejects_zero_capacity` |
| `VMNL_NET_ENOTSUP` | `fails_as_unsupported` |
| `VMNL_NET_ENOBUFS` | `fails_when_full`, `fails_with_more_than_free_space` |
| `VMNL_NET_ENOMEM` | `fails_without_memory`, `fails_without_memory_for_<allocation>` |
| NULL error | `accepts_null_error` |

- the tests go from the nominal case to the properties, the limits, the errors and the nullable parameters, the NULL error last
- the tests check the documented contract, not the implementation: a behavior without documentation is documented before it is tested
- every documented limit is tested exactly at the limit and one past it: a capacity, the free space, the end of a chunk or of a block, an offset equal to the size
- one behavior per test, with `ASSERT` for the steps that set the case up and `EXPECT` for what the test checks
- the tests are independent: each one frees what it creates, its fixture resets the global state, and the `sanitize` preset runs them under AddressSanitizer and UndefinedBehaviorSanitizer
- a `VmnlNetError` starts at `UINT32_MAX`, so the test proves the function writes it
- a test that replaces the allocator restores the default one before its first `ASSERT`, or leaves it to the teardown of its fixture, which runs even after a failed `ASSERT`, since the test binary run on its own runs every test in the same process

```c
BUFFER_FIXTURE_I(vmnl_net_buffer_consume, types, 16)

UTEST_I(vmnl_net_buffer_consume, drops_first_bytes, NTYPES)
{
    VmnlNetError error = UINT32_MAX;

    ASSERT_TRUE(append(utest_fixture->buffer, "hello"));
    EXPECT_TRUE(vmnl_net_buffer_consume(utest_fixture->buffer, 2, &error));
    EXPECT_EQ(error, VMNL_NET_SUCCESS);
    EXPECT_TRUE(data_equals(utest_fixture->buffer, "llo"));
}
```

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

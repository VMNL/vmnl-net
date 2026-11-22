# Quality

The CI checks every pull request with the tools below, and a failure blocks the merge.

## Tools

- clang-format checks the formatting, with the `format-check` preset.
- clang-tidy checks the library on Linux and Windows, with the `tidy` preset (Linux and Windows for `posix` and `win32` dedicated code base). 
- The compilers treat warnings as errors in the CI, through `VMNL_NET_WARNINGS_AS_ERRORS`.
- AddressSanitizer, UndefinedBehaviorSanitizer and MemorySanitizer run the tests.
- gcovr fails below 100 % of the lines and branches covered by the tests.
- Doxygen fails on any documentation warning (like undocumented function).
- The benchmarks build and run once.

## Baseline

> November 21, 2026

clang-tidy entered the CI with [#86](https://github.com/VMNL/vmnl-net/pull/86).
The current `.clang-tidy`, run on `main` just before it (b96de83) on macOS, reports 39 warnings in the library.

- 35 `misc-include-cleaner`, for headers used without being included directly
- 2 `readability-magic-numbers` and 1 `bugprone-signed-bitwise`, in `src/version/version.c`
- 1 `readability-math-missing-parentheses`, in `src/context/context_advance.c`

[#86](https://github.com/VMNL/vmnl-net/pull/86) fixed all of them, and each source now includes the headers it uses.
Since then the count stays at 0, as the CI runs clang-tidy with `--warnings-as-errors`.

## Final measurement

> May 2027

The same command, run at the end of the year.

## Rules

The checks of the `bugprone`, `cert`, `clang-analyzer`, `misc`, `performance`, `portability` and `readability` families are on, and the naming options follow the [conventions](conventions.md).
Five custom checks cover the conventions that no check covers.

- `explicit-comparison` flags a condition that is not a comparison, like `if (ready)` or `if (!pointer)`, and asks to compare it with `== true`, `== false`, `== NULL` or `!= NULL`.
- `conversion-to-bool` asks to compare a value instead of converting it to a bool.
- `sizeof-expression` asks for `sizeof` without parentheses on an expression.
- `ternary-condition` asks for the condition of a ternary in parentheses.
- `const-value` keeps `const` for what a pointer points to and for static tables.

Two checks are off.

- `clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling` flags `memcpy()` or `memmove()` and asks for their bounds-checked versions, like `memcpy_s()`. These come from Annex K of C11, an optional part of the standard. glibc never provided it, as WG14 noted in [N1967](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1967.htm), and macOS only provides `memset_s()`, so the library cannot call them on Linux and macOS.
- `readability-implicit-bool-conversion` flags every comparison the conventions ask for. In C, `true` and `false` are integers, so `vmnl_net_buffer_is_empty(buffer) == false` converts the bool to an integer, and the check has no option to ignore it. `conversion-to-bool` keeps its useful part, the real conversions like `bool set = pointer;`.

`misc-include-cleaner` ignores the system headers whose name starts with an underscore and the internal headers of the Windows SDK, which a source never includes directly.

clang-tidy only checks the library.
On November 22, 2026, the same configuration reported 737 distinct warnings in the tests, 383 in the benchmarks and 19 in the examples, and almost all of them were noise.
The utest.h macros inflate the cognitive complexity and the magic numbers of the tests, the benchmarks are C++ while the configuration targets C, and the examples print with `printf()` without checking its result.

## Technical debt

### Sorted list of timers

> Since October 12, 2026

- Why it stays: the naive list came first, as [decision 0011](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0011-sorted-list-of-timers.md) explains, and [#100](https://github.com/VMNL/vmnl-net/issues/100) compares a wheel and a heap in 0.5.0 for [decision 0012](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0012-timer-structure.md)
- What it costs: a start walks the list with deadlines out of order, which takes 19 to 21 µs with 32,000 active timers in a new process, up to 26 µs after repeated runs in the same process
- What fixing it would cost: to estimate

### One allocation per object

> Since October 21, 2026

- Why it stays: [decision 0008](https://github.com/VMNL/vmnl-net/blob/main/docs/decisions/0008-opaque-handles.md) keeps opaque handles, and pools owned by the context only come once the benchmarks show the need
- What it costs: each buffer and timer is a call to the allocator, which the `buffer_create` and `timer_create` benchmarks measure
- What fixing it would cost: to estimate

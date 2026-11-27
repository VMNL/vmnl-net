# 0013 Mutation testing

- Date: November 24, 2026
- Status: accepted

**Context and problem**

The CI requires the tests to cover 100 % of the lines and branches, so the coverage no longer shows where the tests are weak.
It shows which code runs, not whether a test fails when that code changes.
Mutation testing makes small edits to the code, called mutants, and runs the tests on each one. A mutant that no test catches shows code that runs without being checked.
Some code is only observable on one system. The address module refuses leading zeros by itself, which `inet_pton()` already does on Linux but doesn't on macOS.

**Options considered**

1. The coverage alone, as today.
2. Mull on Linux.
3. Mull on Linux and macOS, with the reports merged.

**Option chosen**

Option 3. A mutant counts as caught if one system catches it.
Mull runs every mutator except the boundary ones, which move a comparison by one, like `>` into `>=`.
The CI fails while a mutant survives on both systems, like the coverage below 100 %, and shows the surviving mutants in the run's summary and report.

**Reason**

On November 24, 2026, Mull 0.39.1 found code that the tests run without checking, at [475f6f4](https://github.com/VMNL/vmnl-net/commit/475f6f4). A resize to the current capacity can reallocate the buffer, and a chunked buffer created with a capacity of 0 can skip writing its error code, without any test failing.
The tests caught 445 of 493 mutants on Linux, 446 of 488 on macOS, and 457 of 498 once merged.
The 7 mutants of the leading zero check survive on Linux and are caught on macOS. Option 2 would report them as gaps, when the tests on macOS check them.
On November 25, 2026, no test could catch 16 of the 47 boundary mutants on macOS. In `(now > start) ? now - start : 0`, `>` and `>=` both give 0 when `now == start`. Each of them would need a comment in the source for Mull to skip it, and the sources carry no such comment, not even for clang-tidy.

**Consequences accepted**

- Mull and LLVM are pinned together, as Mull's plugin is built for one version of LLVM.
- Each system took more than 3 minutes on the main maintainer's machine, with 12 threads. A runner has fewer, and the time grows with the library and its tests.
- A mutant can allocate until the memory runs out, which killed the whole Linux job on the first run. The mutants run with 2 GB of address space each, a limit that Linux applies and macOS ignores.
- A mutant caught on one system counts as caught, even when the other system runs that code without checking it.
- A new surviving mutant blocks the CI until a test catches it or the code changes.
- A comparison moved by one is no longer checked. Testing every limit at its value and one past it rests on the review again, although the boundary mutants found 5 gaps in the tests.
- The `win32` code is not mutated, as Mull does not support Windows.

**Completed on November 27, 2026**

- Mull reads the coverage of the tests, built with `-fprofile-instr-generate -fcoverage-mapping`, and `--include-not-covered` reports a mutant that no test runs as `NoCoverage`.
- The CI accepts these mutants. The coverage already fails below 100 % outside the `GCOVR_EXCL` regions, which hold system failures that no test can cause, like `fcntl()` on an open socket, each with its reason.
- The merge prefers a surviving mutant to an uncovered one, so code that only macOS runs still counts.

The sources are in the [research log](../watch.md).

# 0004 Benchmark comparison on a reference machine

- Date: November 22, 2026
- Status: accepted, supersedes [0003](0003-benchmark-comparison-in-the-ci.md)

**Context and problem**

Three runs of the comparison from [0003](0003-benchmark-comparison-in-the-ci.md) compared a base with a pull request that only changed the CI, so both sides built the same code.

- On November 21, 2026, split over 5 jobs, `buffer_resize/ring/capacity:16384` moved by -13 % and `buffer_clear/chunked/bytes:4096` by -27 %. The 9 repetitions of the first stayed within 1 % on each side, so the test of `compare.py` reported its change as significant, with p < 0.001.
- On November 21, 2026, split over 10 jobs, `timer_set_interval` moved by +22 %. Its base fell from 1.22 ns to 0.978 ns after the fourth repetition, within the same process.
- On November 22, 2026, in a single job of 74 minutes, 18 of the 179 benchmarks moved by more than 5 %, 8 by more than 10 % and 2 by more than 20 %, up to +47 % on `buffer_create/dynamic/capacity:256`.

On the runners, times shift between two processes and even within one.
The test of `compare.py` only sees the spread within each process, so it reports changes on identical code.

**Options considered**

1. Keeping the comparison in the CI, with more repetitions or with the base and pull request runs interleaved.
2. A self-hosted runner on an EIP virtual machine.
3. Counting instructions with Cachegrind in the CI.
4. Comparing on a reference machine, the main maintainer's, with a script.

**Option chosen**

Option 4. `tools/compare_benchmarks.sh` builds the working tree and a base, runs both in turn, 9 times each in a new process, compares them with `compare.py` and plots the report with gnuplot. A run of a commit against itself measures the day's noise.
The CI builds the benchmarks and runs each one once, which catches a benchmark that no longer builds, crashes or skips with an error.

**Reason**

In an Ubuntu 24.04 container on the main maintainer's machine, five runs of the same code gave medians within 3.0 % of each other for `buffer_resize/ring/capacity:16384`, 4.6 % for `timer_set_interval` and 8.8 % for `buffer_clear/chunked/bytes:4096`, against 13 % to 27 % on the runners.
On November 22, 2026, a run of a commit against itself on the same machine moved 3 of the 179 benchmarks by more than 5 % and none by more than 10 %, up to 8.8 % on `buffer_consume/chunked/bytes:65536`, against 47 % on the runners.
The same day, running the two builds in turn, 9 times each in a new process, lowered the benchmarks that the test of `compare.py` reported as changed on identical code from 122 to 4 of 179, against 9 repetitions in a single process. Repetitions in one process miss the variation between processes, as Kalibera and Jones describe in "Rigorous Benchmarking in Reasonable Time" (ISMM 2013).
Option 1 spreads a runner's jumps over both sides but does not lower them.
Option 2 would run the code of any pull request from a fork on the virtual machine, which does not control its host's frequency either.
Option 3 misses cache and memory effects, which decide between the timer structures.

**Consequences accepted**

- Pull requests are no longer compared automatically. The CI only checks that every benchmark builds and runs, and the comparison is run by hand on the reference machine. It takes about an hour for the whole suite, less with a filter.
- The numbers hold for one machine. A contributor can compare on their own machine, but those numbers do not compare with the reference machine's.
- The reference machine's noise changes from day to day, so each comparison runs in the same conditions and starts with a run of a commit against itself.
- Two different binaries also differ in memory layout, which can shift times by itself, as Mytkowicz, Diwan, Hauswirth and Sweeney showed in "Producing Wrong Data Without Doing Anything Obviously Wrong!" (ASPLOS 2009). A change within the day's noise proves nothing.
- The test of `compare.py` still reports a few changes on identical code, 4 of the 179 benchmarks with p < 0.05 on November 22, 2026. Only a change above the noise of the day's run of a commit against itself counts.
- A benchmark can also drift within a process. Over 20 repetitions, `timer_start/random/active:32000` slowed down from 19.9 ms to 26.0 ms per batch, while its first run in 10 new processes stayed between 18.9 ms and 20.9 ms.

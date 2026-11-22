# 0003 Benchmark comparison in the CI

- Date: November 21, 2026
- Status: superseded by [0004](0004-benchmark-comparison-on-a-reference-machine.md)

**Context and problem**

The benchmarks measure each public function, but nothing compares two versions, so a pull request can slow the library down unnoticed.
Times vary between runs, so a plain threshold between two runs would raise false alarms.

**Options considered**

1. Comparing the pull request with its base in the same job, with Google Benchmark's `compare.py`, which runs a Mann-Whitney U test on the repetitions.
2. Storing each run's results in the repository and alerting above a threshold.
3. Counting executed instructions with Cachegrind instead of measuring time.
4. A self-hosted runner, always the same machine.

**Option chosen**

Option 1. On each pull request that changes the code, the benchmarks or the build, the CI builds the benchmarks of the pull request and its base, runs each 9 times on the same runner, and reports the comparison without blocking the merge.
Option 3 ran alongside at first. It was dropped because a whole-suite instruction diff mixed setup with measurement and could not name the benchmark behind a change.

**Reason**

Running both builds on one runner removes the differences between machines, the test of `compare.py` tells a change from noise, and nothing depends on an external service.
Option 2 compares runs from different machines, and option 4 would run the code of any pull request from a fork on a project machine.

**Consequences accepted**

- The job is long, since the 179 benchmarks run 9 times per build, so it only reports and never blocks a merge.

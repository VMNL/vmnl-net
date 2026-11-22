#!/bin/sh
# Compares the benchmarks of the working tree with those of a base commit or branch, then plots the report with gnuplot.

set -eu

ROOT=$(cd "$(dirname "$0")/.." && pwd)
REPORT="$ROOT/build/bench/report"
ROUNDS=9
REPETITIONS=1
WORK=$(mktemp -d)

readonly ROOT REPORT ROUNDS REPETITIONS WORK

trap 'git -C "$ROOT" worktree remove --force "$WORK/base" 2> /dev/null || true; rm -rf "$WORK"' EXIT

if [ "$#" -eq 0 ]; then
    cat >&2 << EOF
USAGE: $0 <base> [benchmark options]

Builds the benchmarks of the working tree and of <base>, a commit or a branch, runs both in turn, $ROUNDS times each in a new process, and compares them with the compare.py tool of Google Benchmark.
The comparison goes to build/bench/report/, as times.txt and as benchmarks-report.pdf, plotted with gnuplot.
The benchmark options go to both runs, like --benchmark_filter to run only some benchmarks.
A full comparison takes about an hour. Run it on a quiet machine, and compare a commit with itself first to measure the noise of the day.
Needs CMake, Git, gnuplot, jq and Python 3.

EXAMPLES:
    $0 main
    $0 HEAD
    $0 main --benchmark_filter=timer_
EOF
    exit 1
fi

for tool in cmake git gnuplot jq python3; do
    if ! command -v "$tool" > /dev/null; then
        echo "$0: $tool is not installed" >&2
        exit 1
    fi
done

git -C "$ROOT" worktree add --detach "$WORK/base" "$1"
shift
(cd "$ROOT" && cmake --workflow --preset bench)
(cd "$WORK/base" && cmake --workflow --preset bench)

python3 -m venv "$WORK/venv"
"$WORK/venv/bin/pip" install --quiet numpy scipy

mkdir -p "$REPORT"
for round in $(seq "$ROUNDS"); do
    "$WORK/base/build/bench/bin/vmnl_net_benchmarks" --benchmark_repetitions="$REPETITIONS" --benchmark_out="$WORK/base-$round.json" --benchmark_out_format=json "$@"
    "$ROOT/build/bench/bin/vmnl_net_benchmarks" --benchmark_repetitions="$REPETITIONS" --benchmark_out="$WORK/head-$round.json" --benchmark_out_format=json "$@"
done
jq -s -f "$ROOT/tools/merge_benchmarks.jq" "$WORK"/base-*.json > "$REPORT/base.json"
jq -s -f "$ROOT/tools/merge_benchmarks.jq" "$WORK"/head-*.json > "$REPORT/head.json"
"$WORK/venv/bin/python" "$ROOT/build/bench/_deps/benchmark-src/tools/compare.py" --no-color -a benchmarks "$REPORT/base.json" "$REPORT/head.json" | tee "$REPORT/times.txt"
gnuplot -c "$ROOT/tools/benchmarks_report.gp" "$REPORT/times.txt" "$REPORT/benchmarks-report.pdf"
echo "$REPORT/benchmarks-report.pdf"

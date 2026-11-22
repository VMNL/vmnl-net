# Merges the runs of one side of a comparison into one result, with the median of all their repetitions.

def median: sort | if length % 2 == 1 then .[(length - 1) / 2] else (.[length / 2 - 1] + .[length / 2]) / 2 end;

[.[].benchmarks[] | select(.run_type == "iteration")] as $runs
| {
    context: .[0].context,
    benchmarks: ($runs + [
        $runs
        | group_by(.name)[]
        | .[0] + {
            name: (.[0].name + "_median"),
            run_type: "aggregate",
            aggregate_name: "median",
            repetitions: length,
            real_time: (map(.real_time) | median),
            cpu_time: (map(.cpu_time) | median)
        }
    ])
}

# Merges the Mull reports of several systems into one, in the mutation-testing-elements format.
# A mutant counts as caught if one system caught it, and a surviving mutant wins over one that no test runs. The paths of the reports must be relative to the repository.
# Usage: jq -s -f tools/merge_mutants.jq linux.json macos.json

def caught: .status == "Killed" or .status == "Timeout";

.[0] + {
    files: (
        [.[].files | to_entries[]]
        | group_by(.key)
        | map({
            key: .[0].key,
            value: (.[0].value + {
                mutants: ([.[].value.mutants[]] | group_by([.mutatorName, .location]) | map((map(select(caught)) + map(select(.status == "Survived")) + .)[0]))
            })
        })
        | from_entries
    )
}
| del(.mutationScore)

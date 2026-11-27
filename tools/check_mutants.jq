# Fails unless every mutant of a merged Mull report is caught, runs on a line that the coverage excludes, or is a reviewed equivalent mutant.
# Usage: jq -e --slurpfile equivalent tools/equivalent_mutants.json -f tools/check_mutants.jq mutation.json

def original($lines): .location as $l
    | if $l.start.line == $l.end.line then $lines[$l.start.line - 1][($l.start.column - 1):($l.end.column - 1)] else "..." end;

$equivalent[0] as $known
| [
    .files
    | to_entries[]
    | .key as $file
    | (.value.source | split("\n")) as $lines
    | .value.mutants[]
    | .status == "Killed" or .status == "Timeout" or .status == "NoCoverage"
      or (original($lines) as $code | .replacement as $replacement | any($known[]; .file == $file and .original == $code and .replacement == $replacement))
  ]
| all

# Writes the Markdown summary of a merged Mull report, with the score of each module and the surviving mutants.
# Usage: jq -r -f tools/summarize_mutants.jq mutation.json

def caught: .status == "Killed" or .status == "Timeout";
def score($caught; $total): if $total == 0 then "-" else "\(($caught * 100 / $total) | floor) %" end;
def cell: gsub("\\|"; "\\|");
def original($lines): .location as $l
    | if $l.start.line == $l.end.line then $lines[$l.start.line - 1][($l.start.column - 1):($l.end.column - 1)] else "..." end;

[
    .files
    | to_entries[]
    | .key as $file
    | (.value.source | split("\n")) as $lines
    | .value.mutants[]
    | {file: $file, module: ($file | split("/")[1]), status, original: original($lines), replacement, line: .location.start.line, column: .location.start.column}
] as $mutants
| ($mutants | length) as $total
| ($mutants | map(select(caught)) | length) as $caught
| ($mutants | map(select(.status == "Timeout")) | length) as $timeouts
| ($mutants | map(select(.status == "NoCoverage")) | length) as $uncovered
| ($mutants | map(select(.status == "Survived"))) as $survived
| "## Mutation testing",
  "",
  "Mull \(.framework.version) caught **\($caught) of \($total)** mutants on Linux and macOS, a score of **\(score($caught; $total))**, with \($timeouts) timeouts counted as caught. \($survived | length) mutants survived, and no test runs \($uncovered) others, on lines that the coverage excludes.",
  "",
  "| Module | Mutants | Caught | Survived | Score |",
  "|---|--:|--:|--:|--:|",
  ($mutants | group_by(.module) | sort_by(-length)[] | (map(select(caught)) | length) as $c | "| \(.[0].module) | \(length) | \($c) | \(map(select(.status == "Survived")) | length) | \(score($c; length)) |"),
  if ($survived | length) == 0 then empty else
      "",
      "<details><summary>\($survived | length) surviving mutants</summary>",
      "",
      "| Location | Original | Mutant |",
      "|---|---|---|",
      ($survived | sort_by(.file, .line, .column)[] | "| `\(.file):\(.line):\(.column)` | `\(.original | cell)` | `\(.replacement | cell)` |"),
      "",
      "</details>"
  end

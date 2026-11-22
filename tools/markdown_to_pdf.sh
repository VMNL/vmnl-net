#!/bin/sh
# Prints Markdown files of the repository to PDF, rendered by Doxygen like the documentation, then printed by Chrome.

set -eu

ROOT=$(cd "$(dirname "$0")/.." && pwd)
STYLE='font: 8pt sans-serif; color: #6f7e8e;'
CHROME=${CHROME:-$(command -v google-chrome || command -v chromium || echo "/Applications/Google Chrome.app/Contents/MacOS/Google Chrome")}
DOXYFILE="$ROOT/build/docs/Doxyfile"
PDF="$ROOT/build/pdf"
WORK=$(mktemp -d)

readonly ROOT STYLE CHROME DOXYFILE PDF WORK

trap 'rm -rf "$WORK"' EXIT

if [ "$#" -eq 0 ]; then
    cat >&2 << EOF
USAGE: $0 <file.md> [...]

Renders Markdown files like the Doxygen documentation and prints each one to a PDF in build/pdf/.
Needs CMake, Doxygen and Chrome. Set CHROME to the Chrome binary if the script does not find it.

EXAMPLE: $0 docs/watch.md docs/decisions/*.md
EOF
    exit 1
fi

for tool in cmake doxygen; do
    if ! command -v "$tool" > /dev/null; then
        echo "$0: $tool is not installed" >&2
        exit 1
    fi
done

if [ ! -x "$CHROME" ]; then
    echo "$0: Chrome is not found, set CHROME to its path" >&2
    exit 1
fi

(cd "$ROOT" && cmake --preset docs > /dev/null)
mkdir -p "$PDF"

css_string() {
    printf '"%s"' "$(printf '%s' "$1" | sed 's/[\\"]/\\&/g')"
}

for file; do
    path=$(cd "$(dirname "$file")" && pwd)/$(basename "$file")
    name=$(basename "$file" .md)
    title=$(sed -n 's/^# //p' "$path" | head -n 1)
    folder=$(dirname "${path#"$ROOT"/}")

    sed -E "s#\]\(([^)\#:][^):]*)\)#](https://github.com/VMNL/vmnl-net/blob/main/$folder/\1)#g" "$path" > "$WORK/$name.md"
    {
        echo "@page {"
        echo "    margin: 2cm 1.5cm;"
        echo "    @top-left { content: $(css_string "$(sed -n 's/^PROJECT_NAME *= *"\([^@"]*\).*/\1/p' "$ROOT/docs/Doxyfile.in") $(sed -n 's/^ *VERSION *//p' "$ROOT/CMakeLists.txt" | head -n 1)"); $STYLE }"
        echo "    @top-right { content: $(css_string "$title | $(LC_ALL=C date '+%B %-d, %Y') | $(git -C "$ROOT" rev-parse --short HEAD)$(git -C "$ROOT" diff --quiet HEAD -- || echo -dirty)"); $STYLE }"
        echo "    @bottom-center { content: \"Page \" counter(page) \" of \" counter(pages); $STYLE }"
        echo "}"
        echo "hr.footer, address.footer { display: none; }"
    } > "$WORK/$name.css"
    {
        cat "$DOXYFILE"
        echo "INPUT = $WORK/$name.md"
        echo "EXCLUDE ="
        echo "USE_MDFILE_AS_MAINPAGE = $WORK/$name.md"
        echo "OUTPUT_DIRECTORY = $WORK/$name"
        echo "GENERATE_TREEVIEW = NO"
        echo "DISABLE_INDEX = YES"
        echo "SEARCHENGINE = NO"
        echo "WARN_AS_ERROR = NO"
        echo "WARN_LOGFILE = $WORK/$name.log"
        echo "HTML_EXTRA_STYLESHEET += $WORK/$name.css"
    } | (cd "$ROOT" && doxygen -)
    "$CHROME" --headless=new --disable-gpu --no-sandbox --no-pdf-header-footer --virtual-time-budget=10000 --print-to-pdf="$PDF/$name.pdf" "file://$WORK/$name/html/index.html" 2> /dev/null
    echo "$PDF/$name.pdf"
done

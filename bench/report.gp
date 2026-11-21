data = "< awk '" . \
    "$1 ~ /_median$/ { median[substr($1, 1, length($1) - 7)] = $2 * 100 } " . \
    "$1 ~ /_pvalue$/ { pvalue[substr($1, 1, length($1) - 7)] = $2 } " . \
    "END { for (name in median) { color = pvalue[name] >= 0.05 ? 3 : (median[name] > 0 ? 1 : 2); print name, median[name], color } }" . \
    "' " . ARG1 . " | sort -k2,2n"

stats data using 2 nooutput
rows = STATS_records

set terminal svg size 960, rows * 13 + 140 font "Helvetica,10" background rgb "white" noenhanced
set output ARG2
set title "Median time change per benchmark, head against base"
set xlabel "faster <- median time change -> slower (grey band: 3%, red or green: p < 0.05)"
set xrange [-30:30]
set yrange [-1:rows]
set xtics 10 format "%+g%%"
set link x2
set x2tics 10 format "%+g%%"
set grid xtics
set key off
set object 1 rectangle from -3, graph 0 to 3, graph 1 fillcolor rgb "#e8e8e8" fillstyle solid noborder behind
set linetype 1 linecolor rgb "#c0392b"
set linetype 2 linecolor rgb "#1e8449"
set linetype 3 linecolor rgb "#999999"

clamp(x) = x < -30 ? -30 : (x > 30 ? 30 : x)

plot data using (0):0:(clamp($2)):(0):3 with vectors nohead linecolor variable, \
     data using (clamp($2)):0:3:ytic(1) with points pointtype 7 pointsize 0.6 linecolor variable

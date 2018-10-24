# To generate plots with UE vectors placed at (-5000, 0, 1) with a 50.0 Mbps
# rate run the following:
# gnuplot -e vectors="'(-5000, 0, 1)'" -e rate="'50.0'" -e suffix="'-5000-0-1'" scripts/lab4-plots.p

if (!exists("vectors")) vectors='(0, 0, 0)'
if (!exists("suffix")) suffix='0-0-0'
if (!exists("rate")) rate='0.0 Mbps'

set format y '%.1s%cbps'
set terminal pngcairo enhanced truecolor size 520,350 fontscale 0.8
set size 1.0, 1.0
set yrange [0:]
set xrange [0:20]
set offset graph 0.0, graph 0.0, graph 0.3, graph 0.1

set style line 1 lt -1 pi -6 pt 7 lc rgb "red" lw 1
set style line 2 lt -1 pi -3 pt 4 lc rgb "orange" lw 1
set style line 3 lt -1 pi -5 pt 5 lc rgb "green" lw 1

set title "PDCP throughput for different antenna types at: ".vectors.", with rate: ".rate." Mbps"
set output "pdcp-throughput-plot_".suffix.".png"
set xlabel "Time (s)"
set ylabel "PDCP Throughput (bps)"
plot "< head -n 101 results/lab4/isotropic/DlPdcpStats.txt | tail -n 100" \
        using ($1):(($10)*8) \
        title "PDCP throughput (Isotropic)" \
        ls 1 \
        with linespoints,\
     "< head -n 101 results/lab4/cosine/DlPdcpStats.txt | tail -n 100" \
        using ($1):(($10)*8) \
        title "PDCP throughput (Cosine)" \
        ls 2 \
        with linespoints,\
     "< head -n 101 results/lab4/parabolic/DlPdcpStats.txt | tail -n 100" \
        using ($1):(($10)*8) \
        title "PDCP throughput (Parabolic)" \
        ls 3 \
        with linespoints

set title "RLC throughput for different antenna types at: ".vectors
set output "rlc-throughput-plot_".suffix.".png"
set xlabel "Time (s)"
set ylabel "RLC Throughput (bps)"
plot "< head -n 101 results/lab4/isotropic/DlRlcStats.txt | tail -n 100" \
        using ($1):(($10)*8) \
        title "RLC throughput (Isotropic)" \
        ls 1 \
        with linespoints,\
     "< head -n 101 results/lab4/cosine/DlRlcStats.txt | tail -n 100" \
        using ($1):(($10)*8) \
        title "RLC throughput (Cosine)" \
        ls 2 \
        with linespoints,\
     "< head -n 101 results/lab4/parabolic/DlRlcStats.txt | tail -n 100" \
        using ($1):(($10)*8) \
        title "RLC throughput (Parabolic)" \
        ls 3 \
        with linespoints

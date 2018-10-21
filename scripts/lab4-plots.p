if (!exists("vectors")) vectors='(0, 0, 0)'

set format y '%.1s%cbps'
set terminal pngcairo enhanced truecolor size 520,350 fontscale 0.8
set size 1.0, 1.0
set offset graph 0.0, graph 0.0, graph 0.1, graph 0.1

set style line 1 lt -1 pi -6 pt 7 lc rgb "red" lw 2
set style line 2 lt -1 pi -3 pt 4 lc rgb "orange" lw 2
set style line 3 lt -1 pi -5 pt 5 lc rgb "green" lw 2

set title "PDCP throughput for different antenna types at: ".vectors
set output "pdcp-throughput-plot.png"
set xlabel "Time (s)"
set ylabel "PDCP Throughput (bps)"
plot "< paste results/lab4/isotropic/DlPdcpStats.txt \
              results/lab4/isotropic/UlPdcpStats.txt \
              | head -n 101 | tail -n 100" \
        using ($1):(($10+$12+$26+$28)*8) \
        title "PDCP throughput (Isotropic)" \
        ls 1 \
        with linespoints,\
     "< paste results/lab4/cosine/DlPdcpStats.txt \
              results/lab4/cosine/UlPdcpStats.txt \
              | head -n 101 | tail -n 100" \
        using ($1):(($10+$12+$26+$28)*8) \
        title "PDCP throughput (Cosine)" \
        ls 2 \
        with linespoints,\
     "< paste results/lab4/parabolic/DlPdcpStats.txt \
              results/lab4/parabolic/UlPdcpStats.txt \
              | head -n 101 | tail -n 100" \
        using ($1):(($10+$12+$26+$28)*8) \
        title "PDCP throughput (Parabolic)" \
        ls 3 \
        with linespoints

set title "RLC throughput for different antenna types at: ".vectors
set output "rlc-throughput-plot.png"
set xlabel "Time (s)"
set ylabel "RLC Throughput (bps)"
plot "< paste results/lab4/isotropic/DlRlcStats.txt \
              results/lab4/isotropic/UlRlcStats.txt \
              | head -n 101 | tail -n 100" \
        using ($1):(($10+$12+$26+$28)*8) \
        title "RLC throughput (Isotropic)" \
        ls 1 \
        with linespoints,\
     "< paste results/lab4/cosine/DlRlcStats.txt \
              results/lab4/cosine/UlRlcStats.txt \
              | head -n 101 | tail -n 100" \
        using ($1):(($10+$12+$26+$28)*8) \
        title "RLC throughput (Cosine)" \
        ls 2 \
        with linespoints,\
     "< paste results/lab4/parabolic/DlRlcStats.txt \
              results/lab4/parabolic/UlRlcStats.txt \
              | head -n 101 | tail -n 100" \
        using ($1):(($10+$12+$26+$28)*8) \
        title "RLC throughput (Parabolic)" \
        ls 3 \
        with linespoints

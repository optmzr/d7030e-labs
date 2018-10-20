set format y '%.0s%cbps'
set title "PDCP throughput for different antenna types"
set xlabel "Time (s)"
set ylabel "PDCP Throughput (bps)"
plot "< head -n 50 results/lab4/isotropic/DlPdcpStats.txt" \
        using ($1):($10*8) \
        title "PDCP throughput (Isotropic)" \
        with linespoints,\
     "< head -n 50 results/lab4/cosine/DlPdcpStats.txt" \
        using ($1):($10*8) \
        title "PDCP throughput (Cosine antenna)" \
        with linespoints,\
     "< head -n 50 results/lab4/parabolic/DlPdcpStats.txt" \
        using ($1):($10*8) \
        title "PDCP throughput (Parabolic antenna)" \
        with linespoints

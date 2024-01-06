#!/bin/bash

make clean && make monte_carlo
./monte_carlo -n 5000 | tail -n +2  > /tmp/graph1.dat

./monte_carlo -n 15000 | tail -n +2 | awk '{print $1" "$2 - 3.14159265359}' > /tmp/graph2L1.dat
./monte_carlo -n 15000 -r 2 | tail -n +2 | awk '{print $1" "$2 - 3.14159265359}' > /tmp/graph2L2.dat
./monte_carlo -n 15000 -r 3 | tail -n +2 | awk '{print $1" "$2 - 3.14159265359}' > /tmp/graph2L3.dat
./monte_carlo -n 15000 -r 4 | tail -n +2 | awk '{print $1" "$2 - 3.14159265359}' > /tmp/graph2L4.dat


gnuplot <<END
    set terminal pdf
    set output "monte_carlo.pdf"
    set title "monte carlo coconuts"
    set xlabel "x"
    set ylabel "y"
    set size square
    set nokey
    set palette model RGB
    set palette defined (0 "blue", 1 "red")
    unset colorbox
    plot "/tmp/monte_carlo.dat" using 3:4:5 ls 7 ps 0.25 palette, \
	 [x=0:1] sqrt(1-x**2) lw 3
END

gnuplot <<END
    set terminal pdf
    set output "monte_carlo2.pdf"
    set logscale x 2
    set yrange [-1:1]
    set xrange [1:16384]
    set title "Monte Carlo Error Estimation"
    set xlabel "Number of Iterations"
    set ylabel "Error"
    set grid lw 2
    set nokey
    plot "/tmp/graph2L1.dat" using 1:2  with lines, \
	 "/tmp/graph2L2.dat" using 1:2 with lines, \
         "/tmp/graph2L3.dat" using 1:2 with lines, \
         "/tmp/graph2L4.dat" using 1:2 with lines
END

gnuplot <<END
    set terminal pdf
    set output "graph3.pdf"
    set boxwidth 1 relative
    set style fill solid
    set yrange [-0.2:0.2]
    set xrange [1:15000]
    set title "Monte Carlo Error Estimation 2"
    set xlabel "Number of Iterations"
    set ylabel "Error"
    set grid lw 2
    set nokey
    plot "/tmp/graph2L2.dat" using 1:2 with boxes
END

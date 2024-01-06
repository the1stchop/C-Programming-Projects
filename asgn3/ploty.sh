#!/bin/bash

gnuplot <<END
    set terminal pdf
    set output "Moves_Elements.pdf"
    set title "Moves Performed"
    set xlabel "Elements"
    set ylabel "Moves"
    set key font ",8"
    plot "shell.dat" using 1:2  with lines lw 1, \
         "quick.dat" using 1:2 with lines lw 1, \
         "heap.dat" using 1:2 with lines lw 1, \
         "batcher.dat" using 1:2 with lines lw 1
END

gnuplot <<END
    set terminal pdf
    set output "Comparisons_Elements.pdf"
    set title "Comparisons Performed"
    set xlabel "Elements"
    set ylabel "Comparisons"
    set key font ",8"
    plot "shell.dat" using 1:3  with lines lw 1, \
         "quick.dat" using 1:3 with lines lw 1, \
         "heap.dat" using 1:3 with lines lw 1, \
         "batcher.dat" using 1:3 with lines lw 1
END


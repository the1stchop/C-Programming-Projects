#!/bin/bash

for i in {1..10000};do
	./sorting -s -p 0 -n $i | awk '{print $3" "$5" "$7}' >> shell.dat
	./sorting -b -p 0 -n $i | awk '{print $3" "$5" "$7}' >> batcher.dat
	./sorting -q -p 0 -n $i | awk '{print $3" "$5" "$7}' >> quick.dat
	./sorting -h -p 0 -n $i | awk '{print $3" "$5" "$7}' >> heap.dat
done

gnuplot <<END
    set terminal pdf
    set output "Moves_Performed.pdf"
    set title "Moves Performed"
    set xlabel "elements"
    set ylabel "moves"
    set size square
    set nokey
    plot "shell.dat" using 1:2 with lines, \
         "batcher.dat" using 1:2 with lines, \
	 "quick.dat" using 1:2 with lines, \
	 "heap.dat" using 1:2 with lines, \
END

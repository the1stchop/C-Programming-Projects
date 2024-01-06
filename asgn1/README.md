# Assignment 1: Getting Aquainted with UNIX and C

## Short Description:
In this assignment we were tasked with writing a bash script, plot.sh, that took the  
datafrom a c program, monte\_carlo.c, and create graphs with that data. To make the graphs,  
plot.sh uses gnuplot. These graphs are then put into pdf files that appear in the directory  
that you ran the program in.

## Build:
To build, just make sure you have the files: Makefile, monte\_carlo.c, and plot.sh, in  
your directory. No need to make any executables because plot.sh will do it for you.

## Cleaning:

To clean the directory after running plot.sh, type '$ make clean' to remove the  
monte\_carlo executable and the .o files from your directory. If you want to run plot.sh  
again, there is no need to run '$ make clean' because plot.sh will clear the directory before  
running.

## Running:
To run plot.sh, type '$ ./plot.sh'. This will run the program and create 3 individual pdfs  
in your directory each containing a different graph using the data from monte\_carlo.

# Assignment 3: Sorting: Putting your affairs in order

## Short Description:
In this assignment we were tasked with creating 4 different sorting algorithms. We also had to make another c program that tests and compares these sorting algorithms with each other. Here is a list of the c programs in this directory:

-shell.c: This file contains the shell\_sort() algorithm. This function sorts an array using a variation of insertion sort. It will sort elements that are far apart, and then sort with a closer gap after every iteration until every element is sorted.

-heap.c: This file contains the heap\_sort() algorithm. Heap sort is implemented as a specialized binary tree and sorts its elements using 2 routines: building and fixing a heap. First, take the array to sort and build a heap from it. Then, fix the heap by removing the top of the heap, and putting it in the end of the array. Then fix the order of the heap, largest elements going to the top.

-quick.c: This file contains the quick\_sort() algorithm. Quick sort uses a divide-and-conquer method to sort arrays. It chooses an element and makes it a pivot. Then it splits the array in two, any elements less than the pivol goes to the left sub-arry and any elements greater go to the right sub-array. Then it will do this process with the sub-arrays and so on until the whole array is sorted.

-sorting.c: This file contains the test cases for all of the sorting algorithms, as well as main(). This will also allow for comparing the 4 sorting algorithms. 

-plot.sh and ploty.sh: These two bash scripts make the graphs for my WRITEUP.pdf. plot.sh makes the data files out of the sorting executable and ploty.sh makes the graphs from those data files. They are already executables so you just have to run ./plot.sh and then ./ploty.sh to get the graphs.

## Build and Clean
To build, type "$make" into the command line. This will create the executable "sorting" and all the .o files required. Then you should be good to go. Running the executable without any other flags will output the commands you can use. To clear your directory, do "$make clean". This will remove the executable made and all .o files.

## Running
to run the executable, type "$ ./sorting". This will print out a brief description of the program as well as all the commands you can input. Then, your all good to go!


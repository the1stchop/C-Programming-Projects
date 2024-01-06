# Assignment 4: The Game of Life

## Short Description:
In this assignment we were tasked with implementing John Horton Conway's, Game of Life. Here is a list of the files in this directory with a short description about them that made it happen:

-universe.c: This file contains our Universe ADT as well as the constructor, accessor, and manipulator functions for out ADT. These functions will help us make, access, and manipulate the universes in our life.c.

-life.c: This file contains the command-option line for users and is where we use the functions from universe.c. 

-Makefile: This file builds our executable and .o files. It also can format our .c files and clean our directory.

## Build and Clean
To build, type "$make" int the command line. This will creat the executable "life" and all the .o files required. Then you should be good to go. To celar your directory, do "$make clean". This will remove the execuable made and all .o files.

##Running
First, to get all the possible commands to run the program, type "$ ./life -h". This will print out a brief description of the program as well as all the commands you can use. Then, your all good to go!

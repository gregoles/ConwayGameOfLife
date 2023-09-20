# Conway's Game of Life

## Description:
The goal of this program was to recreate Conway's Game of Life in C. There is a helper program called universe.c that implemented various functions that would help create and run the universe. The functions would then be used in the main test program called life.c. The program was is meant to take in a number of inputs such as a text file containing the size of the universe in the first row and various x,y points that represent live cells. There are also certain command line options that can be used, such as the number of generations that should be run, the option to silence the ncurses screen, and setting an output file.

## Build:
To build the program run '$ make' or '$ make all' on the command line in the terminal while in the assignment directory. This will create the necessary .o files as well as an executable called life which can be run to run the program

## Clean:
To clean the directory after building the program, type '$ make clean' into the command line. This will remove the executable files as well as the .o files from the directory.

## Command Line Options
-t             Create your universe as a toroidal\
-s             Silent - do not use animate the evolution\
-n {number}    Number of generations [default: 100]\
-i {file}      Input file [default: stdin]\
-o {file}      Output file [default: stdout]

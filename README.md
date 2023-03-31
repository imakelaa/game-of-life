## Short Description:
	This program implements the game of life created by mathematician John Horton Conway.
	It follows the following rules:
	1. Any live cell with two or three live neighbors survives.
	2. Any dead cell with exactly three live neighbors becomes a live cell.
	3. All other cells die, either due to loneliness or overcrowding.

## Build:
	To build/compile this program we can make use of the following commands:
	$ make
	$ make all
	$ make life 
	
	To delete all the files that are compiler generated use the following command:
	$ make clean
	
	To make all the files clang formatted use the following command:
	$ make format
	

## Running:
	SYNOPSIS
    Conway's Game of Life

    USAGE
        ./life tsn:i:o:

    OPTIONS
        -t             Create your universe as a toroidal
        -s             Silent - do not use animate the evolution
        -n {number}    Number of generations [default: 100]
        -i {file}      Input file [default: stdin]
        -o {file}      Output file [default: stdout]



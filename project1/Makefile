#
# Sample Makefile for project 1
#
# The program used for illustration purposes is
# a simple program using prime numbers
#


## CC  = Compiler.
## CFLAGS = Compiler flags.
CC	= gcc
CFLAGS =	-Wall -Wextra -std=gnu99


## OBJ = Object files.
## SRC = Source files.
## EXE = Executable name.

SRC =		main.c disk.c memory.c
OBJ =		main.o disk.o memory.o
EXE = 		swap

## Top level target is executable.
$(EXE):	$(OBJ)
	$(CC) $(CFLAGS) -g -o $(EXE) $(OBJ) -lm


## Clean: Remove object files and core dump files.
clean:
	/bin/rm $(OBJ)

## Clobber: Performs Clean and removes executable file.

clobber: clean
	/bin/rm $(EXE)

## Dependencies

main.o:	functions.h
disk.o:	functions.h
memory.o:	functions.h

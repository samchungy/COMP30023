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

SRC =		swap.c process.c
OBJ =		swap.o process.o
EXE = 		swap

## Top level target is executable.
$(EXE):	$(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) -lm


## Clean: Remove object files and core dump files.
clean:
	/bin/rm $(OBJ)

## Clobber: Performs Clean and removes executable file.

clobber: clean
	/bin/rm $(EXE)

## Dependencies

swap.o:	swap.h process.h
process.o:	process.h

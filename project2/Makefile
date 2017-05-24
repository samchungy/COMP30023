#
# Sample Makefile for project 1
#
# The program used for illustration purposes is
# a simple program using prime numbers
#


## CC  = Compiler.
## CFLAGS = Compiler flags.
CC	= gcc
CFLAGS =	-Wall -pthread -std=gnu99


## OBJ = Object files.
## SRC = Source files.
## EXE = Executable name.

SRC =		server.c Work.c
OBJ =		server.o Work.o
EXE = 		server

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

server.o: server.h
Work.o: Work.h

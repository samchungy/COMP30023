//
// Created by SamCh on 25/05/2017.
//

#ifndef PROJECT2_SOLN_H
#define PROJECT2_SOLN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdint.h>
#include "uint256.h"

void calculate_soln(char *, char *, char *, char*);
void hex_to_byte(int, BYTE *, char *);
int doublehash(BYTE *, BYTE *, char *);
char *printbytes (BYTE *, int);
#endif //PROJECT2_SOLN_H

// Change this file in any way you like
// Do not alter driver.c / Makefile / md.h
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <stdbool.h>

#define BRDSZ 6
#define MAXBRDS 200000

/*
Here define struct state -
it might look something like:

struct board{
   2D array of tiles
   hawk
   parent
}

struct state{
    array of struct board[];
    etc.
}
*/

struct board{
   char board[BRDSZ][BRDSZ];
   int hawk;
   int parent;
}

struct state{
    struct board state[MAXBRDS]; 
    int count;

}


#include "md.h"

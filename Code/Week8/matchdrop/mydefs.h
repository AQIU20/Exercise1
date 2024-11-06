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

#define MAXSTR (BRDSZ*BRDSZ+BRDSZ+2)


typedef struct {
    char tiles[BRDSZ][BRDSZ];
    char hawk;
    int parent;
}board;

 typedef struct  {
    board brd[MAXBRDS];
    int row;
    int col;
} state;


#include "md.h"


#pragma once
#define MAXROWS 20
#define WIDTH 5
#define HEIGHT 6
#define HORIZONTAL 0
#define VERTICAL 1
struct st {
   char board[MAXROWS][WIDTH];
   int row;
   int col;
   int maxHeight;
};
typedef struct st state;





#include "mydefs.h"



bool file2str(const char* fname, char* str) {
   if (fname == NULL || str == NULL) {
      return false;
   }

   FILE *fp = fopen(fname, "r");
   if (fp == NULL) {
      
      return false;
   }

   char line[MAXSTR];
   int isFirstLine = 1; 
   str[0] = '\0';       

   // get first line
   while (fgets(line, sizeof(line), fp) != NULL) {
      line[strcspn(line, "\n")] = '\0';

      if (isFirstLine) {
         
         strcat(str, line);
         isFirstLine = 0;
      } else {
         
         strcat(str, "-");
         strcat(str, line);
      }
   }

   fclose(fp);

   return true;
}



state* str2state(char* str) {
    if (str == NULL) {
        return NULL;
    }

    state *s = (state*)malloc(sizeof(state));
    if (s == NULL) {
        perror("alloc state failed!");
        return NULL;
    }

    s->brd[0].parent = -1;  
    s->row = 0;            
    s->col = 0;            

    // strtok use '-' 
    char* token = strtok(str, "-");

    // store hawk tile
    if (token == NULL || strlen(token) != 1) {
        free(s);
        return NULL;  
    }
    s->brd[0].hawk = token[0];  

    // store col
    int row = 0;
    while ((token = strtok(NULL, "-")) != NULL && row < BRDSZ) {
        int len = strlen(token);
        if (len > BRDSZ) {
            free(s);
            return NULL; 
        }

        if (s->col == 0) {
            s->col = len;  
        } 

        // copy token to tiles
        for (int i = 0; i < len; i++) {
            s->brd[0].tiles[row][i] = token[i];
        }

        row++;
    }

    s->row = row;  

    if (s->row > BRDSZ) {
        free(s);
        return NULL;
    }

    return s;
}


board copyBoard(state* s, board* brd) {
    board newBrd;
    for (int i = 0; i < s->row; i++) {
        for (int j = 0; j < s->col; j++) {
            newBrd.tiles[i][j] = brd->tiles[i][j];
        }
    }

    return newBrd;
}



int printBoard(state* s, int finalIndex, bool verbose) {
    //from finalIndex to parent 
    int chain[100] = {-1};
    int index = 0;

    while(finalIndex != -1) {
        chain[index] = finalIndex;
        finalIndex = s->brd[finalIndex].parent;
        index++;
    }

    if (!verbose) {
        return index - 1;
    }

    //from to tail to head, its the board chain
    //print them out by order
    for (int i = index - 1; i >= 0; i--) {
        printf("%c\n", s->brd[chain[i]].hawk);

        for (int j = 0; j < s->row; j++) {
            for (int k = 0; k < s->col; k++) {
                printf("%c", s->brd[chain[i]].tiles[j][k]);
            }
            printf("\n");  
        }
        
        
        if (i > 0) {
            printf("\n");
        }
    }
    
    return index - 1;
}

bool checkFinalBoard(state* s, board* brd) {

    if (s == NULL || brd == NULL) {
    fprintf(stderr, "Error: Null pointer passed to checkFinalBoard\n");
    return false;
}
    for (int i = 0; i < s->col; i++) {
        for (int j = 1; j < s->row; j++) {
            if (j >= BRDSZ || i >= BRDSZ) {
                fprintf(stderr, "Error: Array index out of bounds in checkFinalBoard\n");
                return false;
            }

            if (brd->tiles[j][i] != brd->tiles[j - 1][i]) {
                return false;
            }

        }
    }
    return true;
}


void insertIntoBoard(state* s,int parentIndex,  int boardlen, board* brd) {
    s->brd[boardlen].hawk = brd->hawk;
    s->brd[boardlen].parent = parentIndex;
    for (int i = 0; i < s->row; i++) {
        for (int j = 0; j < s->col; j++) {
            s->brd[boardlen].tiles[i][j] = brd->tiles[i][j];
        }
    }
}

bool checkEqualBoard(state* s, int index,  board* brd) {
    if (brd->hawk != s->brd[index].hawk) {
        return true;
    }

    for (int i = 0; i < s->row; i++) {
        for (int j = 0; j < s->col; j++) {
            if (brd->tiles[i][j] != s->brd[index].tiles[i][j]) {
                return true;
            }
        }
    }
    return false;

}


bool checkUniqueBoard(state* s, int boardlen, board* brd) {
    for (int i = 0; i < boardlen; i++) {

        if (checkEqualBoard(s, i, brd) == false) {
            return false;
        }
    }

    return true;
}





int solve(state* s, bool verbose)
{
    //index of parentboard and len of board
    int checkIndex = 0;
    int boardlen = 1;
    if (s == NULL) {
        return -1;
    }
    

    while(checkIndex < boardlen) {
        board *parentBoard = &s->brd[checkIndex];
        
        if (checkFinalBoard(s, parentBoard)) {
            return printBoard(s, checkIndex, verbose);
        }
        
        // printf("checkIndex: %d boardLen: %d\n", checkIndex, boardlen);
        // printf("parent board:\n");
        // printf("%c\n", parentBoard->hawk);
        // for (int i = 0; i < s->row; i++) {
        //     for (int j = 0; j < s->col; j++) {
        //         printf("%c ", parentBoard->tiles[i][j]);
        //     }
        //     printf("\n");
        // }
        // printf("\n");



        //get all childBoard
        for (int i = 0; i < s->col; i++) {
            board childBoard = copyBoard(s, parentBoard);
            //swap hawk to childBoard's tiles[s->row - 1][i]
            childBoard.hawk = childBoard.tiles[s->row - 1][i];

            //drop one element for this col: childBoard.tiles[j][i]
            for (int j = s->row - 1; j >= 1; j--) {
                childBoard.tiles[j][i] = childBoard.tiles[j - 1][i];
            }
            childBoard.tiles[0][i] = parentBoard->hawk;
            
            

            //check if this childBoard is unique
            if (checkUniqueBoard(s, boardlen, &childBoard)) {
                insertIntoBoard(s, checkIndex, boardlen, &childBoard);
                boardlen++;
            }

            if (checkFinalBoard(s, &childBoard)) {
                return printBoard(s, boardlen - 1, verbose);
            }

        }


        checkIndex++;
        
    }
   
    return -1;
}



void test(void)
{
    char str[MAXSTR];
    state* s;

    //assert(file2str("2moves.brd", str));
    strcpy(str, "A-ABC-ABC-ABC-CBA");
    s = str2state(str);
    assert(s);
    assert(solve(s, true)==2);
    free(s);


   
}


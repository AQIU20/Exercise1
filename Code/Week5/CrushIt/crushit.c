#include "crushit.h"
#include "mydefs.h"


bool isFormalStr(const char* str) {
    int len = strlen(str);
    if (len % 5 != 0) {
        return false;
    }

    for (int i = 0; i < len; i++) {
        if (str[i] < 'A' || str[i] > 'Z') return false;
    }
    return true;
}


bool initialise(state* s, const char* str)
{
    if (s == NULL || str == NULL) {
        return false;
    }

    FILE *fp = fopen(str, "r");
    memset(s->board, 0, sizeof(s->board));
    char buffer[150] = {0};
    s->row = 0;
    s->col = 5;
    if (fp == NULL) {
        // check if its a board
        if (!isFormalStr(str)) {
            perror("string either a filename or a formal string!\n");
            return false;
        }

        strcpy(buffer, str);
    }else {
        char ch;
        int i = 0;
        while ((ch = fgetc(fp)) != EOF) {
            if (ch == '\n') {
                continue;
            }

            buffer[i] = ch;
            i++;
        }
    }

    s->row = strlen(buffer) / s->col;
    s->maxHeight = s->row;
    for (int i = 0; i < s->row; i++) {
        for (int j = 0; j < s->col; j++) {
            s->board[i][j] = buffer[i * s->col + j];
        }
    }
    printf("row: %d maxHeight: %d\n", s->row, s->maxHeight);
    return true;

}

bool tostring(const state* s, char* str)
{
    if (s == NULL || str == NULL) return false;
    memset(str, 0, sizeof(char));
    int k = 0;
    for (int i = s->row - s->maxHeight; i < s->row; i++) {
        for (int j = 0; j < s->col; j++) {
            str[k] = s->board[i][j];
            k++;
        }
    }
    str[k] = '\0';
    printf("%s\n", str);
    return true;
}

int checkMatch(state *s, int x, int y, int direction) {
    char c = s->board[x][y];
    if (c == '.') {
        return -1;
    }


    int len = 0;
    if (direction == HORIZONTAL) {
        while(y < s->col && s->board[x][y] == c) {
            y++;
            len++;
        }

    } else {
        while(x < s->row && s->board[x][y] == c) {
            x++;
            len++;
        }
    }
    if (len >= 3) return len;
    return -1;
}


bool matches(state* s)
{
    if (s == NULL) {
        return false;
    }

    bool tmpBoard[MAXROWS][WIDTH] = {false};
    int startRow = s->row - HEIGHT;
    bool removeFlag = false;
    // printf("matches\n");
    for (int i = startRow; i < s->row; i++) {
        for (int j = 0; j < s->col; j++) {
            int len = 0;

            //check horizontal
            len = checkMatch(s, i, j, HORIZONTAL);
            if (len != -1) {
                for (int m = 0, n = j; m < len; m++, n++) {
                    tmpBoard[i][n] = true;
                }
            }

            //check vertical
            len = checkMatch(s, i, j, VERTICAL);
            if (len != -1) {
                for (int m = 0, n = i; m < len; m++, n++) {
                    tmpBoard[n][j] = true;
                }
            }

        }
    }

    //remove blocks
    for (int i = startRow; i < s->row; i++) {
        for (int j = 0; j < s->col; j++) {
            if (tmpBoard[i][j] == true) {
                //remove block to hole
                s->board[i][j] = '.';
                removeFlag = true;
            }
        }
    }
    return removeFlag;
}

bool dropblocks(state* s)
{
    if (s == NULL) {
        return false;
    }
    // printf("drop\n");
    
    // scan each column from bottom to top
    // write down the non-hole block and rearrange from bottom to top
    int currentMaxHeight = 0;
    char tmpColumn[MAXROWS] = {0};
    for (int j = 0; j < s->col; j++) {
        int pos = 0;
        memset(tmpColumn, 0, sizeof(tmpColumn));
        for (int i = s->row - 1; i >= 0;i--) {

            //add it to tmpColumn if isn't a hole
            if (s->board[i][j] != '.') {
                tmpColumn[pos] = s->board[i][j];
                pos++;
            }
        }

        if (currentMaxHeight < pos) {
            currentMaxHeight = pos;
        }
        //rearrange
        for (int i = s->row - 1, k = 0; i >= 0;i--, k++) {
            if (k < pos) {
                s->board[i][j] = tmpColumn[k];
            }else {
                s->board[i][j] = '.';
            }
        }

    }
    s->maxHeight = currentMaxHeight;

    return true;

}

void test(void)
{
    state s;
    assert(initialise(&s, "BBBDBCDAACDAABDAABCAABCDABCAAA") == true);
    char str[WIDTH*MAXROWS+1];
    assert(tostring(&s, str) == true);
    assert(strcmp(str,    "BBBDBCDAACDAABDAABCAABCDABCAAA") == 0);
    assert(matches(&s) == true); 
}

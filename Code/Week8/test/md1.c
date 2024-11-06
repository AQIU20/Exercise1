#include "mydefs.h"


bool file2str(const char* fname, char* str)
{

    if (fname == NULL) {
        return false;
    }

    FILE *fp = fopen(fname, "r");

    if(fp == NULL){
      strcpy(str, fp);

    }

    else{
      char ch;
      int i = 0;

      while ((ch = fgetc(fp)) != EOF) {
         if (ch == '\n') {
            ch = '-';
         }
         str[i] = ch;
         i++;
      }

    }


}



state* str2state(const char* str)
{

    if (str == NULL || strlen(str) == 0) {
        return NULL; 
    }

    state* s = (state*)malloc(sizeof(state));
    if (s == NULL) {
        return NULL; 
    }

    s->count = 0;

    int count = s->count;
    int row = 0, col = 0;
    s->state[count].hawk = str[0];

    for(int i=2; i<strlen(str); i++){

      if(str[i] > 'A' &&  str[i] < 'Z'){
         s->state[count].board[row][col]= str[i];
         col++;

      }
      else{
         if(str[i] == '-'){
            col = 0;
            row++;
         }

         if(str[i] != '-'){
            return NULL;
         }

      }

      
    }

    return s;



}

int solve(state* s, bool verbose)
{
}

void test(void)
{
}

/* Many of other functions, as required */

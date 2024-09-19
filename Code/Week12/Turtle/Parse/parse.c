#include "parse.h"
#include <stdio.h>



int main(int argc, char** argv) {
    if (argc > 1) {
        printf("File name: %s\n", argv[1]);
    }else {
        printf("No file name given\n");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Could not open file\n");
        exit(EXIT_FAILURE);
    }

    Program *prog = calloc(1, sizeof(Program));
     if(prog==NULL){
      printf("Cannot calloc() space\n"); 
      exit(EXIT_FAILURE);
      
   }

    prog->cw = 0;
    char tmp[100];
    while (fgets(tmp, 100, fp) != NULL) {
        if (tmp[0] == '#') continue;
        int len = strlen(tmp) - 1;
        if (tmp[len] == '\n') {
            tmp[len] = '\0';
        }
        strcat(prog->wds, tmp);
        strcat(prog->wds, " ");
    }
    prog->wds[strlen(prog->wds) - 1] = '\0';//读取结束
    printf("%s\n", prog->wds);

    Prog(prog);
    printf("Parsed OK\n");
}


void Prog(Program* p) {
    SKIP_SPACES
    LOG("PROG");

    if (!strsame(p->wds + p->cw, "START", 1)) {
        return;
    }
    p->cw+=5;
    INSLST(p);
}

void INSLST(Program* p) {
    SKIP_SPACES
    LOG("INSTRCTS");
    if (strsame(p->wds + p->cw, "END", 1)) {
        LOG("INSTRCTS:END");
        p->cw+=3;
        return;
    }
    INS(p);
    SKIP_SPACES
    INSLST(p);
}


void INS(Program* p) {
    SKIP_SPACES
    LOG("INS");
    int cw = p->cw;
    if(FWD(p)) {
        return;
    }
    p->cw = cw;
    if(RGT(p)){
        return;
    }
    p->cw = cw;
    if(COL(p)){
        return;
    }
    p->cw = cw;
    if(LOOP(p)){
        return;
    }
    p->cw = cw;
    if(SET(p)){
        return;
    }
    p->cw = cw;
}


bool FWD(Program* p) {
    SKIP_SPACES
    LOG("FWD");
        if (strsame(p->wds + p->cw, "FORWARD", 7)) {
        p->cw += 7;
        return VARNUM(p);
    }
    return false;
}

bool RGT(Program* p) {
    SKIP_SPACES
    LOG("RGT");
        if (strsame(p->wds + p->cw, "RIGHT", 5)) {
        p->cw += 5;
        return VARNUM(p);
    }
    return false;
}

bool COL(Program* p) {
    SKIP_SPACES
    LOG("COL");
 
        if (strsame(p->wds + p->cw, "COLOUR", 6)) {
        p->cw += 6;

        if(VAR(p)){
            return true;
        }
        else if(WORD(p)){
            return true;
        }
     
    }
    return false;
}

bool LOOP(Program* p) {
    SKIP_SPACES
    LOG("LOOP");
    if (!strsame(p->wds + p->cw, "LOOP", 4)) {
        return false;
    }
    p->cw += 4;
    SKIP_SPACES;
    if(!LTR(p)){
         return false;
    }
    SKIP_SPACES;
    
    if (!strsame(p->wds + p->cw, "OVER", 4)){
         return false;
    }
    p->cw +=4;
    
    SKIP_SPACES;

    if(!LST(p)){
          return false;
    }
    SKIP_SPACES;

    INSLST(p);
    return true;
    
                
}

bool SET(Program* p) {
    SKIP_SPACES
    LOG("SET");
    if (!strsame(p->wds + p->cw, "SET", 3)) {
        return false;
    }
    p->cw += 3;
    SKIP_SPACES;
    if(!LTR(p)){
        return false;
    }
    SKIP_SPACES;
    if(!strsame(p->wds + p->cw, "(", 1)){
        return false;
    }
    p->cw++;
    SKIP_SPACES;
    if(!PFIX(p)){
        return false;

    }
    return true;
    
}

bool VARNUM(Program* p) {
    SKIP_SPACES
    LOG("VARNUM");
    int cw=p->cw;
    if (VAR(p)) {
        return true;
    } 
    p->cw = cw;
    if (NUM(p)) {
            return true;
    }
    p->cw = cw;
    return false;
}


bool VAR(Program* p) {
    SKIP_SPACES
    LOG("VAR");
    if (strsame(p->wds + p->cw, "$", 1)) {
        p->cw++;
            return LTR(p);
        }
    return false;
}

bool LTR(Program* p) {
    SKIP_SPACES
    LOG("LTR");
    if (p->wds[p->cw] >= 'A' && p->wds[p->cw] <= 'Z') {
        p->cw += 1;
        return true;
    }
  
    return false;
}



bool NUM(Program* p) {
    SKIP_SPACES
    LOG("NUM");

    char *endptr;
    strtod(p->wds + p->cw, &endptr);  
    
    if (endptr == p->wds + p->cw) {
        return false;  
    } else {
        int num = endptr - (p->wds + p->cw);  
        p->cw += num;  

        return true;  
    }
}


bool WORD(Program* p) {
    SKIP_SPACES
    LOG("WORD");

    if (p->wds[p->cw] != '"') {
        return false;
      
    }
    int i = p->cw + 1;
    while(p->wds[i] != '"' && p->wds[i] != '\0') {
        i++;
    }
    if ( p->wds[i] == '"') {
      p->cw = i + 1;
      return true;
    }
    p->cw=i-1;
    
    return false;
}


bool LST(Program* p) {
    SKIP_SPACES
    LOG("LST");

    if (strsame(p->wds + p->cw, "{", 1)) {
        p->cw += 1;
        if(ITEMS(p)){
            return true;
        }
    }

    return false;
}


bool ITEMS(Program* p) {
    SKIP_SPACES
    LOG("ITEMS");
    if (strsame(p->wds + p->cw, "}", 1)) {
        p->cw += 1;
        return true;
    }
    else if(!ITEM(p)){
        return false;
    }
    SKIP_SPACES
    if(!ITEMS(p)){

        return false;
    }
    return true;
   
}

bool ITEM(Program* p) {
    SKIP_SPACES
    LOG("ITEM");
    if (VARNUM(p)) {
        return true;
    }
    else if(WORD(p)){
        return true;
    }
        
    return false;
}

bool PFIX(Program* p) {
    SKIP_SPACES
    LOG("PFIX");
    if (strsame(p->wds + p->cw, ")", 1)) {
        p->cw++;
        return true;
    }

    else if(OP(p)){
        LOG("OP");
        PFIX(p);

    }

    else if(VARNUM(p)){
        LOG("VARNUM");
        PFIX(p);

    }

         return false;
    
    
}

bool OP(Program* p) {
    SKIP_SPACES
    LOG("OP");
    if (strsame(p->wds + p->cw, "+", 1)) {
        p->cw += 1;
        return true;
    }
    else if(strsame(p->wds + p->cw, "-", 1)){
        p->cw += 1;
        return true;
    }
    else if(strsame(p->wds + p->cw, "*", 1)){
        p->cw += 1;
        return true;
    }
    else if(strsame(p->wds + p->cw, "/", 1)){
        p->cw += 1;
        return true;
    }    
    return false;
}


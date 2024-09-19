#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAXNUMTOKENS 10000
#define strsame(A,B,N) (strncmp(A, B, N) == 0)
#define LOG(X) printf("%s: %s cw:%d\n", X, p->wds + p->cw, p->cw);
#define SKIP_SPACES \
    while(p->wds[p->cw] == ' ') { \
        p->cw++; \
    } 

struct prog {
    char wds[MAXNUMTOKENS];
    int cw; // current word
};

typedef struct prog Program;

void Prog(Program* p);
void INSLST(Program* p);
void INS(Program* p);
bool FWD(Program* p);
bool RGT(Program* p);
bool COL(Program* p);
bool LOOP(Program* p);
bool SET(Program* p);

bool VARNUM(Program* p);
bool VAR(Program* p);
bool LTR(Program* p);
bool NUM(Program* p);
bool WORD(Program* p);
bool LST(Program* p);
bool ITEMS(Program* p);
bool ITEM(Program* p);
bool PFIX(Program* p);
bool OP(Program* p);
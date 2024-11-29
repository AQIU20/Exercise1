#include "t27.h"
#define MAXUPNODESIZE 50000
#define MAXSTR 50
// 'Internal' function prototypes 
// ...

enum {
    WORDCOUNT, NODECOUNT, MOSTCOMMON
}mode;

int charToInt(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a';
    }

    if (c >= 'A' && c <= 'Z') {
        return c - 'A';
    }

    //'''
    return ALPHA - 1;

}

char int_to_char(int x) {
    if (x == ALPHA - 1) {
        return '\'';
    }
    return x + 'a';
}



dict* dict_init(void)
{
    dict *d = (dict*)malloc(sizeof(dict));
    if (d == NULL) return d;
    d->freq = 0;
    d->terminal = false;
    d->up = NULL;
    for (int i = 0; i < ALPHA; i++) {
        d->dwn[i] = NULL;
    }


    return d;
}

bool dict_addword(dict* p, const char* wd)
{
    if (p == NULL || wd == NULL) {
        return false;
    }

    dict *tmp = p;
    int len = strlen(wd);
    for (int i = 0; i < len; i++) {
        int c = charToInt(wd[i]);
        if (tmp->dwn[c] == NULL) {
            dict* d = dict_init();
            tmp->dwn[c] = d;
            tmp->dwn[c]->freq = 0;
            tmp->dwn[c]->terminal = false;
            tmp->dwn[c]->up = tmp;
        }

        tmp = tmp->dwn[c];
    }

    tmp->freq++;
    if (tmp->terminal) {
        return false;
    }

    tmp->terminal = true;
    return true;
}


void dict_free(dict** d)
{
    if (d == NULL || *d == NULL) {
        return;
    }

    for (int i = 0; i < ALPHA; i++) {
        if ((*d)->dwn[i] != NULL) {
            dict_free(&((*d)->dwn[i]));
        }
    }
    free(*d);
    *d = NULL;
}

void traverseTree(int* ret, const dict* p, int mode) {
    if (p == NULL) {
        return;
    }
 
    if (mode == WORDCOUNT && p->terminal) {
        *ret += p->freq;
    }else if (mode == NODECOUNT) {
        (*ret)++;
    }else if (mode == MOSTCOMMON && p->terminal && p->freq > *ret) {
        *ret = p->freq;
    }

    for (int i = 0; i < ALPHA; i++) {
        if (p->dwn[i]) {
            traverseTree(ret, p->dwn[i], mode);
        }
    }
}

int dict_wordcount(const dict* p)
{
    //traverse
    int sum = 0;
    traverseTree(&sum, p, WORDCOUNT);
    return sum;
}

int dict_nodecount(const dict* p)
{
    //traverse
    int sum = 0;
    traverseTree(&sum, p, NODECOUNT);
    return sum;
}

dict* dict_spell(const dict* p, const char* str)
{
    if (p == NULL || str == NULL) {
        return NULL;
    }
    dict* tmp = NULL;
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        int c = charToInt(str[i]);
        if (i == 0) {
            if (p->dwn[c] == NULL) {
                return NULL;
            }
            tmp = p->dwn[c];
        }else {
            if (tmp->dwn[c] == NULL) {
            return NULL;
            }
            tmp = tmp->dwn[c];
        }
    }

    if (tmp->terminal) return tmp;
    return NULL;
}

int dict_mostcommon(const dict* p)
{
     //traverse
    int ret = 0;
    traverseTree(&ret, p, MOSTCOMMON);
    return ret;
}

// CHALLENGE1
unsigned dict_cmp(dict* p1, dict* p2)
{
    // find all parents' node from p1 and p2
    dict* group1[MAXUPNODESIZE], *group2[MAXUPNODESIZE];
    int len1 = 0, len2 = 0;
    dict* tmp1 = p1, *tmp2 = p2;
    while(tmp1 != NULL) {
        group1[len1] = tmp1;
        tmp1 = tmp1->up;
        len1++;
    }

    while(tmp2 != NULL) {
        group2[len2] = tmp2;
        tmp2 = tmp2->up;
        len2++;
    }

    // if p1 in p2's parent node group, or p2 in p1's parent node group,
    //which means its the parent itself.
    for (int i = 0; i < len1; i++) {
        if (group1[i] == p2) {
            //p2 is p1's parent node
            return i;
        }
    }

    for (int i = 0; i < len2; i++) {
        if (group2[i] == p1) {
            //p1 is p2's parent node
            return i;
        }
    }


    // thet are in different branch, so find their closest parents.
    for (int i = 0; i < len1; i++) {
        for (int j = 0; j < len2; j++) {
            if (group1[i] == group2[j]) {
                // printf("%d %d\n", i, j);
                return i + j;
            }
        }
    }


    return 7;
}


void find_freq(const dict* p, char* max_freq_str, char* cnt, int* max_freq) {
    if (p == NULL) {
        return;
    }

    for (int i = 0; i < ALPHA; i++) {
        if (p->dwn[i] == NULL) {
            continue;
        }

        int len = strlen(cnt);
        cnt[len] = int_to_char(i);
        cnt[len + 1] = '\0';

        if (p->dwn[i]->terminal && p->dwn[i]->freq >= *max_freq) {
            if (p->dwn[i]->freq > *max_freq || strcmp(cnt, max_freq_str) < 0) {
                *max_freq = p->dwn[i]->freq;
                strcpy(max_freq_str, cnt);
            }
        }
        find_freq(p->dwn[i], max_freq_str, cnt, max_freq);
        cnt[len] = '\0';
    }

}


// CHALLENGE2
/* CHALLENGE2
   For dictionary 'p', and word 'wd', find the
   path down to the most frequently used word
   below this node, adding these letters to 'ret'.
   (In the event of ties, use the word that comes
   first alphabetically). */
void dict_autocomplete(const dict* p, const char* wd, char* ret)
{
    // 1. find the wd's node
    int wd_len = strlen(wd);
    for (int i = 0; i < wd_len; i++) {
        int c = charToInt(wd[i]);
        p = p->dwn[c];
    }

    // travse tree to find most freq's word
    char cnt[MAXSTR];
    cnt[0] = '\0';
    int max_freq = 0; 
    find_freq(p, ret, cnt, &max_freq);
}

void test(void)
{
}

/* A Extension implementation
   of the Dictionary ADT */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// 26 letters, plus the '
#define ALPHA 27

typedef struct Node {
    char character;               
    struct Node* next;           
    struct Node* child;           
    bool terminal;                
    int freq;                     
} Node;


// You'd normally not expose this structure
// to the user, and it's members should *never*
// be used in e.g. driver.c
struct dict {
    char character;              
    struct dict* next;            
    struct dict* prev;           
    struct dict* child;          
    bool terminal;                
    int freq;                     
};
typedef struct dict dict;

// Creates new dictionary
dict* dict_init(void);

/* Top of Dictionary = p,
   add word str. Return false
   if p or str is NULL, or if word
   is already in the dictionary.
   True otherwise.
*/
bool dict_addword(dict* p, const char* wd);

/* Total number of times that any words
   have been added into the tree =
   sum of freq count of all terminals.*/
int dict_wordcount(const dict* p);

/* Returns the dict* where str is
   marked as 'terminal', or else NULL.
*/
dict* dict_spell(const dict* p, const char* str);

/* Frees all memory used by dictionary p.
   Sets the original pointer back to NULL */
void dict_free(dict** p);

/* Returns number of times most common
   word in dictionary has been added
   (when you insert a word and it already
   exists, increment the count)
*/
int dict_mostcommon(const dict* p);

void test(void);

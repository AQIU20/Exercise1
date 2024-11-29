#include "ext.h"

dict* dict_init(void) {
    dict* node = (dict*)malloc(sizeof(dict));
    if (node == NULL) {
        return NULL; 
    }

    node->character = '\0';  
    node->next = NULL;
    node->prev = NULL;
    node->child = NULL;
    node->terminal = false;
    node->freq = 0;

    return node;
}


bool dict_addword(dict* root, const char* word) {
    if (root == NULL || word == NULL) return false;

    dict* current = root;

    for (int i = 0; word[i] != '\0'; i++) {
        char c = tolower(word[i]); 
        dict* child = current->child;
        dict* prev = NULL;

        
        while (child != NULL && child->character != c) {
            prev = child;
            child = child->next;
        }

        
        if (child == NULL) {
            child = dict_init();
            if (child == NULL) return false; 

            child->character = c;

            
            if (prev) {
                prev->next = child;
                child->prev = prev;
            } else {
                current->child = child; 
            }
        }

        current = child;
    }

    
    if (current->terminal) {
        current->freq++;
        return false; 
    }

    
    current->terminal = true;
    current->freq = 1;

    return true;
}


int dict_wordcount(const dict* root) {
    if (root == NULL) return 0;

    int count = 0;

    
    if (root->terminal) {
        count += root->freq;
    }

    
    count += dict_wordcount(root->child);
    count += dict_wordcount(root->next);

    return count;
}



dict* dict_spell(const dict* root, const char* word) {
    if (root == NULL || word == NULL) return NULL;

    const dict* current = root;

    for (int i = 0; word[i] != '\0'; i++) {
        char c = tolower(word[i]); 
        dict* child = current->child;

      
        while (child != NULL && child->character != c) {
            child = child->next;
        }

        if (child == NULL) {
            return NULL; 
        }

        current = child;
    }

    return current->terminal ? (dict*)current : NULL;
}


void dict_free(dict** root) {
    if (root == NULL || *root == NULL) return;

    dict* current = *root;

    
    dict_free(&(current->child));
    dict_free(&(current->next));

    free(current);
    *root = NULL; 
}


int dict_mostcommon(const dict* root) {
    if (root == NULL) return 0;

    int max_freq = root->terminal ? root->freq : 0;

   
    int child_freq = dict_mostcommon(root->child);
    int next_freq = dict_mostcommon(root->next);

    
    if (child_freq > max_freq) max_freq = child_freq;
    if (next_freq > max_freq) max_freq = next_freq;
    // printf("%d\n", max_freq);

    return max_freq;
}

void test(void)
{
}


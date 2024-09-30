#include "rot18.h"

void rot(char str[]){
    int i=0;

    while(str[i]!='\0'){
        if(str[i]>='a' && str[i]<='z'){
            str[i] =(str[i] - 'a' + 13) % 26 + 'a';
        }

        if(str[i]>='A' && str[i]<='Z'){
            str[i] =(str[i] - 'A' + 13) % 26 + 'A';
        }

        if(str[i]>='0' && str[i]<='9'){
            str[i] =(str[i] - '0' + 5) % 10 + '0';
        }
        i++;

    }

}



void your_tests(void){
    char str[100];

    //Border situation

    strcpy(str, "Az09");
    rot(str);
    assert(strcmp(str, "Nm54") == 0);
    printf("Success\n");

    
}
#include "string.h"
#include "../kernel/io/print.h"

void strrev(char* str){
    int len = strlen(str);
    
    for(int i = 0; i < (len / 2); i++){
        char tmp = str[len - i - 1];

        str[len - i - 1] = str[i];
        str[i] = tmp;
    }
}

int strlen(char* str){
    int len = 0;

    for(len; str[len] != STRING_TERMINATOR; len++);
    return len;
}

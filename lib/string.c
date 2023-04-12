#include "string.h"
#include "../kernel/io/print.h"
#include <stdbool.h>

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

void tolower(char* str){
    while(*str != STRING_TERMINATOR){
        if(*str >= 'A' && *str <= 'Z')
            (*str) += ('a' - 'A'); //convert char to lowercase

        str++;
    }
}

void toupper(char* str){
    while(*str != STRING_TERMINATOR){
        if(*str >= 'a' && *str <= 'z')
            (*str) += ('A' - 'a'); //convert char to UPPERCASE

        str++;
    }
}

int strcmp(char* str1, char* str2){
    while(true){
        if(*str1 > *str2)
            return STRCMP_STR1_IS_BIGGER;
        if(*str2 > *str1)
            return STRCMP_STR2_IS_BIGGER;
        if(*str1 == *str2 && *str1 == 0)
            return STRCMP_EQUALS;

        str1++;
        str2++;
    }
}

void strcpy(char* dst, char* src){
    int i = 0;
    for(i; src[i] != STRING_TERMINATOR; i++){
        dst[i] = src[i];
    }
    dst[i] = STRING_TERMINATOR;
}
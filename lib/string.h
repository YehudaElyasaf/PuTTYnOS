#pragma once

#define STRING_TERMINATOR '\0'
#include <stdbool.h>

//reverse
void strrev(char* str);
//calculate length
int strlen(char* str);
//convert string to lowercase
void tolower(char* str);
//convert string to UPPERCASE
void toupper(char* str);

//string compare
enum STRCMP{
    STRCMP_STR1_IS_BIGGER = 1,
    STRCMP_EQUALS = 0,
    STRCMP_STR2_IS_BIGGER = -1
};
int strcmp(char* str1, char* str2);
void strcpy(char* dst, char* src);
bool isDigit(char ch);
bool isInteger(char* str);

#include "print.h"

int putchar(char ch){
    return cputchar(ch, DEFAULT_COLOR, DEFAULT_COLOR);
}
int cputchar(char ch, uint8_t color, uint8_t backgroundColor){
    return syscall(SYSCALL_PUTCHAR, ch, color, backgroundColor, 0);
}

//private function
static void printString(char* str, uint8_t color, uint8_t backgroundColor){
    while(*str != '\0'){
        cputchar(*str, color, backgroundColor);
        str++;
    }
}

__attribute__((__cdecl__)) int printf(char* format, /*uint8_t color, uint8_t backgroundColor,*/ ...){
    void* arg = &format;
    arg += sizeof(format); //point to start os arguments

    uint8_t color = DEFAULT_COLOR, backgroundColor = DEFAULT_COLOR;
    if(format[0] == PRINTF_SPECIFIER && format[1] == PRINTF_SPECIFIER_CPRINT){
        //color print
        color = (*(uint32_t*)arg);
        arg += sizeof(uint32_t);
        format++;
        backgroundColor = (*(uint32_t*)arg);
        arg += sizeof(uint32_t);
        format++;
    }

    while(*format != '\0'){
        cputchar(*format, color, backgroundColor);
        format++;
    }
}

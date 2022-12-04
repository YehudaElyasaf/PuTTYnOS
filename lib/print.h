#pragma once

#include <stdint.h>
#include "syscall.h"
#include "../kernel/io/print.h"

//return: kernel syscall return
int putchar(char ch);
int cputchar(char ch, uint8_t color, uint8_t backgroundColor);

#define PRINTF_SPECIFIER '%'
enum PrintfSpecifiers{
    PRINTF_SPECIFIER_CPRINT      = 'C',
    PRINTF_SPECIFIER_CHAR        = 'c',
    PRINTF_SPECIFIER_DECIMAL     = 'd',
    PRINTF_SPECIFIER_STRING      = 's',
    PRINTF_SPECIFIER_HEXADECIMAL = 'h',
    PRINTF_SPECIFIER_BINARY      = 'b'
};
/*
to color print - start format string with "%C" (C is uppercase),
and add two uint8_t parameters for color and background color.

printf specifiers:
    %C - print colored (must be in format's beginning)
    %c - char
    %d - int
    %s - string
    %h - hex
    %b - binary

    \n - newline
    \t - indent
*/
__attribute__((__cdecl__)) int printf(char* format, /*uint8_t color, uint8_t backgroundColor,*/ ...);

#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    char* name;
    char* description;
    void (*address)(void);
    struct Command* next;
} Command;


int shellMain();
void runProgram();
void showHelp();

#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    char* name;
    char* description;
    char* usage; //for example: for kill - '<pid> [<pid>...]'
    void (*address)(void);
    struct Command* next;
} Command;

#define BAD_USAGE_EXIT_CODE 0xbad

int shellMain();
void runProgram();
void shellShowHelp();
void shellClear();
void shellKillProcess(int argc, char** argv);

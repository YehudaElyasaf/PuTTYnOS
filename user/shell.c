#include "shell.h"
#include "../lib/ascii.h"
#include "../lib/printf.h"
#include "../lib/scanf.h"
#include "../lib/string.h"
#include <stdbool.h>

//TODO: use syscall, it's user mode
#include "../kernel/tasking/task.h"

static inline void printShellEntry(){
    int SHELL_COLOR = GRAY;
    cputchar('\n', SHELL_COLOR, DEFAULT_COLOR);
    cputchar(244, SHELL_COLOR, DEFAULT_COLOR);
    cputchar('P', LIGHT_RED, DEFAULT_COLOR);
    cputchar('u', LIGHT_BLUE, DEFAULT_COLOR);
    cputchar('T', LIGHT_RED, DEFAULT_COLOR);
    cputchar('T', LIGHT_RED, DEFAULT_COLOR);
    cputchar('Y', LIGHT_RED, DEFAULT_COLOR);
    cputchar('n', LIGHT_BLUE, DEFAULT_COLOR);
    cputchar('O', WHITE, DEFAULT_COLOR);
    cputchar('S', WHITE, DEFAULT_COLOR);
    
    char* folder = "/";
    printf("%C %s", CYAN, DEFAULT_COLOR, folder);
    
    cputchar('\n', SHELL_COLOR, DEFAULT_COLOR);
    cputchar(212, SHELL_COLOR, DEFAULT_COLOR);
    for(int i=0;i<strlen("PuTTynOS ") + strlen(folder); i++)
        cputchar(205, SHELL_COLOR, DEFAULT_COLOR);
    cputchar(181, SHELL_COLOR, DEFAULT_COLOR);

    uint8_t color = GREEN;
}



void t1(){
    while (true)
    {
        printf("4");
        for(long i=0;i<99999; i++);
    }
    
}
void t2(){
    while (true)
    {
        printf("2");
        for(long i=0;i<99999; i++);
    }
}

int shellMain(){
    int a, b, c, d;
    a=createTask(&t1);
    b=createTask(&t2);
    c=createTask(&t1);
    d=createTask(&t2);
    
    //clearScreen();
    printPuTTYnOS();

    while (true)
    {
        printShellEntry();

        char command[100] = {0};
        
        char programName[100] = {0};
        char programArg[100] = {0};
        
        int i = 0;
        char* ch = command;
        scanf("%100s", command);

        //trim spaces in beginning
        for(i = 0; *ch == ' '; i++) ch++;

        for(i = 0; *ch != ' ' && *ch != '\0'; i++){
            programName[i] = *ch;
            ch++;
        }
        
        //trim spaces in middle
        for(ch; *ch == ' '; ch++);

        for(i = 0; *ch != '\0'; i++){
            programArg[i] = *ch;
            ch++;
        }

        //trim spaces in end
        for(--i; programArg[i] == ' '; i--){
            programArg[i] = '\0';
        }


        if(strcmp(programName, "") == STRCMP_EQUALS)
            //no program
            continue;


        printf("\nRunning program %s", programName);
        if(strcmp(programArg, "") != STRCMP_EQUALS)
            printf(" with argument %s", programArg);
        printf("!");
            
        printf("\n");
    }
    
    return 0;
}

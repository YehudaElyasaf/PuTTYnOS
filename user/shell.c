#include "shell.h"
#include "../lib/ascii.h"
#include "../lib/printf.h"
#include <stdbool.h>

static inline printShellEntry(){
    int SHELL_COLOR = GRAY;
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

int shellMain(){
    clearScreen();
    printPuTTYnOS(0);

    while (true)
    {
        printShellEntry();

        char a[100] = {0};
        //scanf("%s", a);

        printf("%C\n%s", RED, DEFAULT_COLOR, a);
        
        for(long i =0;i<99999999; i++);

        putchar('\n');
    }
    
}

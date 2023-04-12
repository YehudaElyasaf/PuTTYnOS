#include "shell.h"
#include "../lib/ascii.h"
#include "../lib/printf.h"
#include "../lib/scanf.h"
#include "../lib/string.h"
#include "../lib/power.h"
#include "../lib/tasking.h"
#include "../kernel/network/network.h"
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

int shellMain(){
    clearScreen();
    printPuTTYnOS(0);

    while (true)
    {
        printShellEntry();

        char command[BUFFER_LEN] = {0};
        
        char programName[BUFFER_LEN] = {0};
        char programArg[BUFFER_LEN] = {0};
        
        int i = 0;
        char* ch = command;
        scanf("%1024s", command);

        //trim spaces in beginning
        for(i = 0; *ch == ' ' || *ch == '\t'; i++) ch++;

        for(i = 0; *ch != ' ' && *ch != '\t' && *ch != '\0'; i++){
            programName[i] = *ch;
            ch++;
        }
        
        //trim spaces in middle
        for(ch; *ch == ' ' || *ch == '\t'; ch++);

        for(i = 0; *ch != '\0'; i++){
            programArg[i] = *ch;
            ch++;
        }

        //trim spaces in end
        for(--i; programArg[i] == ' ' || programArg[i] == '\t'; i--){
            programArg[i] = '\0';
        }

        if(strcmp(programName, "") == STRCMP_EQUALS)
            //no program
            continue;

        runProgram(programName);
        printf("\n");
    }
    
    return 0;
}

void runProgram(char* programName){
        uint32_t* programAddress = NULL;

        if(strcmp(programName, "reboot") == 0){
            reboot();
        }
        else if(strcmp(programName, "shutdown") == 0){
            shutdown();
        }
        else if(strcmp(programName, "help") == 0){
            showHelp();
        }
        else if(strcmp(programName, "arp") == 0){
            printARPTable(2);
        }
        
        else{
            printf("%C\nCommand '%s' not found!", LIGHT_RED, DEFAULT_COLOR, programName);
        }
}

inline static void printCommand(char* commandName, char* description){
    printf("%C\t%s ", LIGHT_GREEN, DEFAULT_COLOR, commandName);
    
    int indent = 10 - strlen(commandName);
    for(int i=0; i < indent; i++)
        printf("%C%c", DARK_GRAY, DEFAULT_COLOR, 196);

    printf("%C %s\n", YELLOW, DEFAULT_COLOR, description);
}
void showHelp(){
    printf("\nUsage: help <command> <parameters>\n");
    printCommand("help", "show system manual");
    printCommand("reboot", "reboot the system");
    printCommand("shutdown", "power off the system");
    printCommand("arp", "show ARP table");
}
#include "shell.h"
#include "../lib/ascii.h"
#include "../lib/printf.h"
#include "../lib/scanf.h"
#include "../lib/string.h"
#include "../lib/power.h"
#include "../lib/tasking.h"
#include "../lib/memory.h"
#include "../lib/heap.h"
#include "../kernel/network/network.h"

//TODO: use syscall, it's user mode
#include "../kernel/tasking/task.h"

static Command* commandsHead;

static void printShellEntry(){
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
    printf("> ", SHELL_COLOR, DEFAULT_COLOR);

    uint8_t color = GREEN;
}


static void addCommand(char* name, char* description, uint32_t* programAddress){
    //allocate command
    Command* newCommand = alloc(sizeof(Command));
    newCommand->name = alloc(strlen(name) + 1);
    newCommand->description = alloc(strlen(description) + 1);

    strcpy(newCommand->name, name);
    strcpy(newCommand->description, description);
    newCommand->address = programAddress;
    newCommand->next = NULL;

    //insert command
    if(commandsHead == NULL)
        commandsHead = newCommand;
    else{
        Command* mov = commandsHead;
        while(mov->next)
            mov = mov->next;

        mov->next = newCommand;
    }
}

int shellMain(){
    commandsHead = NULL;

    addCommand("help", "show system manual", shellShowHelp);
    addCommand("reboot", "reboot the system", reboot);
    addCommand("shutdown", "power off the system", shutdown);
    addCommand("arp", "show ARP table", printARPTable);
    addCommand("clear", "clear the screen", shellClear);

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
        printf("\n");

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
    }
    
    return 0;
}

void runProgram(char* programName){
        uint32_t* programAddress = NULL;

        Command* mov = commandsHead;
        while(mov){
            if(strcmp(mov->name, programName) == 0){
                int processId = createProcess(mov->address, 0, 0); //TODO: params
                join(processId);
                return;
            }
            mov = mov->next;
        }
        printf("%CCommand '%s' not found!\n", RED, DEFAULT_COLOR, programName);
        printf("%CType 'help' to see the command list.\n\n", GRAY, DEFAULT_COLOR, programName);
}

static void printCommand(char* commandName, char* description){
    printf("%C\t%s ", LIGHT_GREEN, DEFAULT_COLOR, commandName);
    
    int indent = 10 - strlen(commandName);
    for(int i=0; i < indent; i++)
        printf("%C%c", DARK_GRAY, DEFAULT_COLOR, 196);

    printf("%C %s\n", YELLOW, DEFAULT_COLOR, description);
}

void shellShowHelp(){
    printf("To run a command, type: <command> [<parameters>]\n");

    Command* mov = commandsHead;
    while(mov){
        printf("%C\t%s ", LIGHT_GREEN, DEFAULT_COLOR, mov->name);
        
        int indent = 10 - strlen(mov->name);
        for(int i=0; i < indent; i++)
            printf("%C%c", DARK_GRAY, DEFAULT_COLOR, 196);

        printf("%C %s\n", YELLOW, DEFAULT_COLOR, mov->description);

        mov = mov->next;
    }

    printf("\n");
    exit(0);
}

void shellClear(){
    clearScreen();
    exit(0);
}
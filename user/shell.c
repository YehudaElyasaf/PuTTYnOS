#include "shell.h"
#include "../lib/ascii.h"
#include "../lib/printf.h"
#include "../lib/scanf.h"
#include "../lib/string.h"
#include "../lib/convert.h"
#include "../lib/power.h"
#include "../lib/tasking.h"
#include "../lib/memory.h"
#include "../lib/heap.h"

//TODO: use syscall, it's user mode
#include "../kernel/network/network.h"
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


static void addCommand(char* name, char* description, uint32_t* programAddress, char* usage){
    //allocate command
    Command* newCommand = alloc(sizeof(Command));
    newCommand->name = alloc(strlen(name) + 1);
    newCommand->description = alloc(strlen(description) + 1);
    newCommand->usage = alloc(strlen(usage) + 1);

    strcpy(newCommand->name, name);
    strcpy(newCommand->description, description);
    strcpy(newCommand->usage, usage);
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

    addCommand("help", "show system manual", shellShowHelp, "[<command>]");
    addCommand("reboot", "reboot the system", reboot, "");
    addCommand("shutdown", "power off the system", shutdown, "");
    addCommand("arp", "show ARP table", printARPTable, "");
    addCommand("clear", "clear the screen", shellClear, "");
    addCommand("tasks", "show runnings tasks", printProcessList, "");
    addCommand("kill", "stop execution of a task", shellKillProcess, "<pid> [<pid>...]");

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
        
        runProgram(programName, programArg);
    }
    
    return 0;
}

int parseArguments(char* strToParse, char** argv){
    int argc = 0;
    char buffer[BUFFER_LEN];

    for(argc; *strToParse != STRING_TERMINATOR; argc++){
        while(*strToParse == ' ')
            strToParse++;
        
        strcpy(buffer, strToParse);
        int i = 0;
        while(buffer[i] != STRING_TERMINATOR && buffer[i] != ' ')
            i++;
        buffer[i] = STRING_TERMINATOR;
        strToParse += i; //goto next argument

        //now the buffer stores the next argument
        argv[argc] = alloc(strlen(buffer) + 1);
        strcpy(argv[argc], buffer);
    }

    return argc;
}

void runProgram(char* programName, char* programArg){
        uint32_t* programAddress = NULL;
        char* argv[10] = { NULL };
        int argc = parseArguments(programArg, argv);

        Command* mov = commandsHead;
        while(mov){
            if(strcmp(mov->name, programName) == 0){
                int processId = createProcess(mov->address, argc, argv, mov->name); //TODO: params
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

void shellShowHelp(int argc, char** argv){
    if(argc == 0){
        printf("To run a command, type: <command> [<parameters>]\n");

        Command* mov = commandsHead;
        while(mov){
            printf("%C\t%s ", LIGHT_GREEN, DEFAULT_COLOR, mov->name);
            
            int indent = 10 - strlen(mov->name);
            for(int i=0; i < indent; i++)
                printf("%C%c", DARK_GRAY, DEFAULT_COLOR, 196);

            printf("%C\t%s\n", YELLOW, DEFAULT_COLOR, mov->description);

            mov = mov->next;
        }

        printf("%C\nType 'help <command>' to see command usage and info.\n\n", CYAN, DEFAULT_COLOR);
        exit(0);
    }
    else if(argc == 1){
        Command* mov = commandsHead;
        while(mov){
            if(strcmp(mov->name, argv[0]) == STRCMP_EQUALS){
                printf("%C%s", LIGHT_GREEN, DEFAULT_COLOR, mov->name);
                printf("%C - ", GRAY, DEFAULT_COLOR);
                printf("%C%s\n\n", YELLOW, DEFAULT_COLOR, mov->description);

                printf("%CUsage:\n%s %s\n\n", LIGHT_CYAN, DEFAULT_COLOR, mov->name, mov->usage);

                exit(0);
            }
            
            mov = mov->next;
        }

        //if we landed here, command not found
        printf("%CCommand '%s' not found!\n", RED, DEFAULT_COLOR, argv[0]);
        printf("%CType 'help' to see the command list.\n\n", GRAY, DEFAULT_COLOR);
        exit(1);
    }
    
    //more than one parameter
    exit(BAD_USAGE_EXIT_CODE);
}

void shellClear(){
    clearScreen();
    exit(0);
}

void shellKillProcess(int argc, char** argv){
    if(argc < 1)
        exit(BAD_USAGE_EXIT_CODE);

    for(int i = 0; i<argc; i++){
        if(strlen(argv[i]) > 9){
            printf("%C%s is not an valid pid.\n\n", RED, DEFAULT_COLOR, argv[i]);
        }
        char pid[10];
        strcpy(pid, argv[i]);
        if(!isInteger(pid) || stoi(pid) < 1){
            printf("%C%s is not an valid pid.\n\n", RED, DEFAULT_COLOR, pid);
            exit(1);
        }
        else if(kill(stoi(pid))){
            exit(0);
        }
        else{
            printf("%CProcess %s doesn't exist.\n\n", RED, DEFAULT_COLOR, pid);
            exit(2);
        }
    }
}
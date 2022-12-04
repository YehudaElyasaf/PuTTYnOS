#include "shell.h"
#include "../lib/ascii.h"
#include "../kernel/io/screen.h"

void shellMain(){
    kprint("\nRunning shell!\n");
    clearScreen();
    printPuTTYnOS(0);
}

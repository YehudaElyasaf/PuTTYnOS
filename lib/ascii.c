#include "ascii.h"
#include "print.h"

//some characters in word PuTTYnOS are in the same column,
//therefore I manually change their color
static int inline getSpecificCharacterColor(int color, int row, int col){
    //toRed chars
    if(
        row == 6    && col == 36    ||
        row == 6    && col == 52    ||
        row == 6    && col == 53    ||
        row == 7    && col == 35    ||
        row == 7    && col == 36    ||
        row == 7    && col == 53    ||
        row == 8    && col == 52
    )
        color = RED;

    //toBlue chars
    if(
        row == 10   && col == 58
    )
        color = BLUE;

    return color;
}

enum PuTTYnOSIndexes{
    BEAR_END_INDEX  =   23,
    P_END_INDEX     =   29,
    u_END_INDEX     =   36,
    TTY_END_INDEX   =   51,
    n_END_INDEX     =   57
};
//print one line according to PuTTYnOS's colors
static void inline cprintPuTTYnOSLine(char* line, int lineIndex, int indent){
    //print indent spaces
    for(int j = 0; j < indent; j++)
    putchar(' ');

    int color;
    for(int i = 0; line[i] != '\0'; i++){
        if(i <= BEAR_END_INDEX)
            color = BROWN;
        else if(i <= P_END_INDEX)
            color = RED;
        else if(i <= u_END_INDEX)
            color = BLUE;
        else if(i <= TTY_END_INDEX)
            color = RED;
            else if(i <= n_END_INDEX)
            color = BLUE;
        else
            color = WHITE;
        color = getSpecificCharacterColor(color, lineIndex, i);
        
        //print character
        cputchar(line[i], color, DEFAULT_COLOR);
    }
}
void printPuTTYnOS(int indent){
    int line = 0;
    cprintPuTTYnOSLine("                __                                                     \n"   , line, indent);  line++;
    cprintPuTTYnOSLine("     __/~~\\-''- _ |                                                    \n"    , line, indent);  line++;
    cprintPuTTYnOSLine("__- - {            \\                                                   \n"    , line, indent);  line++;
    cprintPuTTYnOSLine("     /             \\                                                   \n"    , line, indent);  line++;
    cprintPuTTYnOSLine("    /       ;o    o }                                                  \n"     , line, indent);  line++;
    cprintPuTTYnOSLine("    |              ;                                                   \n"     , line, indent);  line++;
    cprintPuTTYnOSLine("                   '     ____       _____ _______   __      ___  ____  \n"     , line, indent);  line++;
    cprintPuTTYnOSLine("       \\_       (..)    |  _ \\ _   |_   _|_   _\\ \\ / /___  / _ \\/ ___| \n", line, indent);  line++;
    cprintPuTTYnOSLine("         ''-_ _ _ /     | |_) | | | || |   | |  \\ V // _ \\| | | \\___ \\\n" , line, indent);  line++;
    cprintPuTTYnOSLine("           /            |  __/| |_| || |   | |   | || | | | |_| |___) |\n"     , line, indent);  line++;
    cprintPuTTYnOSLine("          /             |_|    \\__,_||_|   |_|   |_||_| |_|\\___/|____/ \n"   , line, indent);  line++;
}
/*
                __      
     __/~~\-''- _ |     
__- - {            \    
     /             \    
    /       ;o    o }   
    |              ;    
                   '     ____       _____ _______   __      ___  ____  
       \_       (..)    |  _ \ _   |_   _|_   _\ \ / /___  / _ \/ ___| 
         ''-_ _ _ /     | |_) | | | || |   | |  \ V // _ \| | | \___ \ 
           /            |  __/| |_| || |   | |   | || | | | |_| |___) |
          /             |_|    \__,_||_|   |_|   |_||_| |_|\___/|____/ 
*/

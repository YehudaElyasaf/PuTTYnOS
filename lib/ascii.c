#include "ascii.h"
#include "printf.h"

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
        color = LIGHT_RED;

    //toBlue chars
    if(
        row == 10   && col == 58
    )
        color = LIGHT_BLUE;

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
            color = LIGHT_RED;
        else if(i <= u_END_INDEX)
            color = LIGHT_BLUE;
        else if(i <= TTY_END_INDEX)
            color = LIGHT_RED;
            else if(i <= n_END_INDEX)
            color = LIGHT_BLUE;
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


void print42(){
    /*
    printf("%C       444444444  ", YELLOW, DEFAULT_COLOR); printf("%C222222222222222    ", LIGHT_CYAN, DEFAULT_COLOR);
    printf("%C      4::::::::4  ", YELLOW, DEFAULT_COLOR); printf("%C:::::::::::::::22  ", LIGHT_CYAN, DEFAULT_COLOR);
    printf("%C     4:::::::::4  ", YELLOW, DEFAULT_COLOR); printf("%C::::::222222:::::2 ", LIGHT_CYAN, DEFAULT_COLOR);
    printf("%C    4::::44::::4  ", YELLOW, DEFAULT_COLOR); printf("%C222222     2:::::2 ", LIGHT_CYAN, DEFAULT_COLOR);
    printf("%C   4::::4 4::::4  ", YELLOW, DEFAULT_COLOR); printf("%C           2:::::2 ", LIGHT_CYAN, DEFAULT_COLOR);
    printf("%C  4::::4  4::::4  ", YELLOW, DEFAULT_COLOR); printf("%C           2:::::2 ", LIGHT_CYAN, DEFAULT_COLOR);
    printf("%C 4::::4   4::::4  ", YELLOW, DEFAULT_COLOR); printf("%C        2222::::2  ", LIGHT_CYAN, DEFAULT_COLOR);
    printf("%C4::::444444::::444", YELLOW, DEFAULT_COLOR); printf("%C   22222::::::22   ", LIGHT_CYAN, DEFAULT_COLOR);
    printf("%C4::::::::::::::::4", YELLOW, DEFAULT_COLOR); printf("%C 22::::::::222     ", LIGHT_CYAN, DEFAULT_COLOR);
    printf("%C4444444444:::::444", YELLOW, DEFAULT_COLOR); printf("%C2:::::22222        ", LIGHT_CYAN, DEFAULT_COLOR);
    printf("%C          4::::4  ", YELLOW, DEFAULT_COLOR); printf("%C:::::2             ", LIGHT_CYAN, DEFAULT_COLOR);
    printf("%C          4::::4  ", YELLOW, DEFAULT_COLOR); printf("%C:::::2             ", LIGHT_CYAN, DEFAULT_COLOR);
    printf("%C          4::::4  ", YELLOW, DEFAULT_COLOR); printf("%C:::::2       222222", LIGHT_CYAN, DEFAULT_COLOR);
    printf("%C        44::::::44", YELLOW, DEFAULT_COLOR); printf("%C::::::2222222:::::2", LIGHT_CYAN, DEFAULT_COLOR);
    printf("%C        4::::::::4", YELLOW, DEFAULT_COLOR); printf("%C::::::::::::::::::2", LIGHT_CYAN, DEFAULT_COLOR);
    printf("%C        4444444444", YELLOW, DEFAULT_COLOR); printf("%C2222222222222222222", LIGHT_CYAN, DEFAULT_COLOR);
    */
}

/*
       444444444   222222222222222    
      4::::::::4  2:::::::::::::::22  
     4:::::::::4  2::::::222222:::::2 
    4::::44::::4  2222222     2:::::2 
   4::::4 4::::4              2:::::2 
  4::::4  4::::4              2:::::2 
 4::::4   4::::4           2222::::2  
4::::444444::::444    22222::::::22   
4::::::::::::::::4  22::::::::222     
4444444444:::::444 2:::::22222        
          4::::4  2:::::2             
          4::::4  2:::::2             
          4::::4  2:::::2       222222
        44::::::442::::::2222222:::::2
        4::::::::42::::::::::::::::::2
        444444444422222222222222222222
*/
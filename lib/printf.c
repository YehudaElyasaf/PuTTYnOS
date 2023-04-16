#include "printf.h"
#include "convert.h"
#include "string.h"

int putchar(char ch){
    return cputchar(ch, DEFAULT_COLOR, DEFAULT_COLOR);
}
int cputchar(char ch, uint8_t color, uint8_t backgroundColor){
    return syscall(SYSCALL_PUTCHAR, ch, color, backgroundColor, 0);
}

//private function
static void printString(char* str, uint8_t color, uint8_t backgroundColor){
    while(*str != '\0'){
        cputchar(*str, color, backgroundColor);
        str++;
    }
}

__attribute__((__cdecl__)) int printf(char* format, /*uint8_t color, uint8_t backgroundColor,*/ /*<type> <varName> ...*/ ...){
    int* pArgumnet = (int*)(&format); //point to the first argument
    pArgumnet++; //point to the first OPTIONAL argument

    uint8_t color = DEFAULT_COLOR, backgroundColor = DEFAULT_COLOR;
    if(format[0] != '\0' && format[0] == PRINTF_SPECIFIER && format[1] == PRINTF_SPECIFIER_CPRINT){
        //color print
        color = (*(uint32_t*) pArgumnet);
        pArgumnet++;
        format++;
        backgroundColor = (*(uint32_t*) pArgumnet);
        pArgumnet++;
        format++;
    }
    
    while(*format != '\0'){
        char conversionBuffer[100] = { 0 }; //to store converted numbers

        if(*format == PRINTF_SPECIFIER){
            format++;
            switch (*format)
            {
            case PRINTF_SPECIFIER_CHAR:
                cputchar(*pArgumnet, color, backgroundColor);
                
                pArgumnet++;
                break;

            case PRINTF_SPECIFIER_DECIMAL:
                itoa(*pArgumnet, conversionBuffer);
                printString(conversionBuffer, color, backgroundColor);

                pArgumnet++;
                break;
            
            //4 different hex prints
            case PRINTF_SPECIFIER_HEXADECIMAL_LOWERCASE:
                itoh(*pArgumnet, conversionBuffer);
                tolower(conversionBuffer);
                printString("0x", color, backgroundColor);
                printString(conversionBuffer, color, backgroundColor);

                pArgumnet++;
                break;
            case PRINTF_SPECIFIER_HEXADECIMAL_UPPERCASE:
                itoh(*pArgumnet, conversionBuffer);
                printString("0x", color, backgroundColor);
                printString(conversionBuffer, color, backgroundColor);

                pArgumnet++;
                break;

            case PRINTF_SPECIFIER_HEXADECIMAL_LOWERCASE_NOPREFIX:
                itoh(*pArgumnet, conversionBuffer);
                tolower(conversionBuffer);
                printString(conversionBuffer, color, backgroundColor);

                pArgumnet++;
                break;
            case PRINTF_SPECIFIER_HEXADECIMAL_UPPERCASE_NOPREFIX:
                itoh(*pArgumnet, conversionBuffer);
                printString(conversionBuffer, color, backgroundColor);

                pArgumnet++;
                break;
                
            case PRINTF_SPECIFIER_BINARY:
                itob(*pArgumnet, conversionBuffer);
                printString(conversionBuffer, color, backgroundColor);

                pArgumnet++;
                break;

            case PRINTF_SPECIFIER_STRING:
                printString((char*)(*pArgumnet), color, backgroundColor);

                pArgumnet++;
                break;

            case PRINTF_SPECIFIER:
                cputchar(PRINTF_SPECIFIER, color, backgroundColor);
                break;
            
            default:
                //specifier doesn't exists, print % and ignore specifier
                cputchar(PRINTF_SPECIFIER, color, backgroundColor);
                format--; //shouldn't has been inreased
                break;
            }
        }
        else{
            //no specifier
            cputchar(*format, color, backgroundColor);
        }

        format++;
    }
}

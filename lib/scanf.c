#include "scanf.h"
#include "printf.h"
#include "convert.h"
#include "memory.h"

#define BUFFER_LEN 512

char getchar() {
    return syscall(SYSCALL_GETCHAR, 0, 0, 0, 0);
}

__attribute__((__cdecl__)) int scanf(char* format, /* <type>* <ptrName> ...*/ ...) {
    int** pArgumnet = (int**)(&format); //point to the first argument, which is A POINTER!!!
    pArgumnet++; //point to the first OPTIONAL argument

    char conversionBuffer[BUFFER_LEN] = { 0 }; //to store converted numbers
    char tmp = 0;

    while(*format != '\0') {
        memcpy(0, conversionBuffer, BUFFER_LEN);

        if(*format == PRINTF_SPECIFIER) {
            format++;
            // I need to know whether the string is dynamic or not and what's its length.
            // when you use %s, you need to specify the length of the string before s or write -1 for it to be dynamic.
            int strLength = stoi(format);
            strLength = strLength < BUFFER_LEN ? strLength : BUFFER_LEN;
            for (; strLength && *format != 's'; format++); // skip the strlength after you know it's a string.
            
            switch (*format)
            {
            case PRINTF_SPECIFIER_CHAR:
                **pArgumnet = getchar();
                pArgumnet++;
                break;

            case PRINTF_SPECIFIER_DECIMAL:
                format++; // point to the stopping character
                for (int i = 0; i < BUFFER_LEN && conversionBuffer[i-1] != *format; i++) {
                    conversionBuffer[i] = getchar();
                    putchar(conversionBuffer[i]);
                }
                
                **pArgument = stoi(conversionBuffer);

                pArgumnet++;
                break;
            
            //4 different hex types
            case PRINTF_SPECIFIER_HEXADECIMAL_LOWERCASE:
            case PRINTF_SPECIFIER_HEXADECIMAL_UPPERCASE:
            case PRINTF_SPECIFIER_HEXADECIMAL_LOWERCASE_NOPREFIX:
            case PRINTF_SPECIFIER_HEXADECIMAL_UPPERCASE_NOPREFIX:
                format++; // point to the stopping character
                for (int i = 0; i < BUFFER_LEN && conversionBuffer[i-1] != *format; i++) {
                    conversionBuffer[i] = getchar();
                    putchar(conversionBuffer[i]);
                }
                
                **pArgument = stoh(conversionBuffer);

                pArgumnet++;
                break;

            case PRINTF_SPECIFIER_STRING:
                int i = 0;
                format++; // point to the stopping character
                for(; i < strLength && conversionBuffer[i-1] != *format; i++) {
                    conversionBuffer[i] = getchar();
                    putchar(conversionBuffer[i]);
                }
                conversionBuffer[--i] = 0;

                memcpy(conversionBuffer, *pArgumnet, i);
                pArgumnet++;
                break;

            case PRINTF_SPECIFIER:
            default:
                //no specifier, wait for a char that is in the format specified, which is '%'.
                tmp = 0;
                while (tmp != PRINTF_SPECIFIER) {
                    tmp = getchar();
                    putchar(tmp);
                }
                break;
            }
        }
        else{
            //no specifier, wait for a char that is in the format specified
            tmp = 0;
            while (tmp != *format) {
                tmp = getchar();
                putchar(tmp);
            }
        }

        format++;
    }
}

#include "scanf.h"
#include "printf.h"
#include "convert.h"
#include "memory.h"

#define BUFFER_LEN 512

#define RELEASE_SC 0x80
#define LSHIFT_SC 0x2a
#define RSHIFT_SC 0x36
#define CAPSLOCK_SC 0x3a
#define NEQ_FMT(a, fmt) (*fmt && a != fmt) || (a != '\n' && !*fmt)
const char *scancode_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6", 
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E", 
        "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "LCtrl", 
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`", 
        "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", 
        "/", "RShift", "Keypad *", "LAlt", "Spacebar"};

const char scancode_ascii[] = { '?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '?', '\t', 'q', 'w', 'e', 'r', 't', 'y', 
        'u', 'i', 'o', 'p', '[', ']', '\n', '?', 'a', 's', 'd', 'f', 'g', 
        'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v', 
        'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '};

const char scancode_caps_ascii[] = { '?', '?', '!', '@', '#', '$', '%', '^',     
    '&', '*', '(', ')', '_', '+', '?', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 
        'U', 'I', 'O', 'P', '{', '}', '\n', '?', 'A', 'S', 'D', 'F', 'G', 
        'H', 'J', 'K', 'L', ':', '"', '~', '?', '|', 'Z', 'X', 'C', 'V', 
        'B', 'N', 'M', '<', '>', '?', '?', '?', '?', ' '};


uint8_t capsl = 0;
uint8_t caps = 0;

char getchar() {
    uint8_t scancode = syscall(SYSCALL_GETCHAR, 0, 0, 0, 0);
    
    while (!scancode || scancode == 1 || scancode >= sizeof(scancode_ascii) || scancode_ascii[scancode] == '?') {
        if (scancode == LSHIFT_SC || scancode == RSHIFT_SC) {
            caps = 1;
        }
        else if (scancode == LSHIFT_SC + RELEASE_SC || scancode == RSHIFT_SC + RELEASE_SC) {
            caps = capsl;
        }
        else if (scancode == CAPSLOCK_SC) {
            capsl = capsl ? 0 : 1;
            caps = capsl ? capsl : caps;
        }
        scancode = syscall(SYSCALL_GETCHAR, 0, 0, 0, 0);
    }

    return caps ? scancode_caps_ascii[scancode] : scancode_ascii[scancode];
}

__attribute__((__cdecl__)) int scanf(char* format, /* <type>* <ptrName> ...*/ ...) {
    int** pArgument = (int**)(&format); //point to the first argument, which is A POINTER!!!
    pArgument++; //point to the first OPTIONAL argument

    char conversionBuffer[BUFFER_LEN] = { 0 }; //to store converted numbers
    char tmp = 0;

    while(*format != '\0') {
        memset(0, conversionBuffer, BUFFER_LEN);

        if(*format == PRINTF_SPECIFIER) {
            format++;
            // I need to know whether the string is dynamic or not and what's its length.
            // when you use %s, you need to specify the length of the string before s or write -1 for it to be dynamic.
            int strLength = stoi(format);
            strLength = strLength < BUFFER_LEN ? strLength : BUFFER_LEN;
            for (; strLength && *format != 's'; format++); // skip the strlength after you know it's a string.
            
            switch (*format) {
            case PRINTF_SPECIFIER_CHAR:
                tmp = getchar();
                putchar(tmp);
                **pArgument = tmp;
                pArgument++;
                break;

            case PRINTF_SPECIFIER_DECIMAL:
                format++; // point to the stopping character
                for (int i = 0; i < BUFFER_LEN && NEQ_FMT(conversionBuffer[i-1], format); i++) {
                    conversionBuffer[i] = getchar();
                    putchar(conversionBuffer[i]);
                }
                
                **pArgument = stoi(conversionBuffer);
                
                pArgument++;
                break;
            
            //4 different hex types
            case PRINTF_SPECIFIER_HEXADECIMAL_LOWERCASE:
            case PRINTF_SPECIFIER_HEXADECIMAL_UPPERCASE:
            case PRINTF_SPECIFIER_HEXADECIMAL_LOWERCASE_NOPREFIX:
            case PRINTF_SPECIFIER_HEXADECIMAL_UPPERCASE_NOPREFIX:
                format++; // point to the stopping character
                for (int i = 0; i < BUFFER_LEN && NEQ_FMT(conversionBuffer[i-1], format); i++) {
                    conversionBuffer[i] = getchar();
                    putchar(conversionBuffer[i]);
                }
                
                **pArgument = stoh(conversionBuffer);

                pArgument++;
                break;

            case PRINTF_SPECIFIER_STRING:
                format++; // point to the stopping character
                for (int i = 0; i < strLength && NEQ_FMT(conversionBuffer[i-1], format); i++) {
                    conversionBuffer[i] = getchar();
                    putchar(conversionBuffer[i]);
                }
                conversionBuffer[strlen(conversionBuffer)-2] = 0;

                memcpy(conversionBuffer, *pArgument, strlen(conversionBuffer)-2);
                pArgument++;
                break;

            case PRINTF_SPECIFIER:
            default:
                //no specifier, wait for a char that is in the format specified, which is '%'.
                tmp = 0;
                while (NEQ_FMT(tmp, format)) {
                    tmp = getchar();
                    putchar(tmp);
                }
                break;
            }
        }
        else{
            //no specifier, wait for a char that is in the format specified
            tmp = 0;
            while (NEQ_FMT(tmp, format)) {
                tmp = getchar();
                putchar(tmp);
            }
        }

        format++;
    }
}
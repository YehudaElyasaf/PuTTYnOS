#include "scanf.h"
#include "printf.h"
#include "string.h"
#include "convert.h"
#include "memory.h"

#define BUFFER_LEN 512

#define RELEASE_SC 0x80
#define LSHIFT_SC 0x2a
#define RSHIFT_SC 0x36
#define CAPSLOCK_SC 0x3a
#define BACKSPACE_SC 0x0e
#define NEQ_FMT(a, fmt) (*fmt && a != *fmt) || (a != '\n' && !*fmt)

#define PRINTN(x) {putchar(x/100%10+'0'); putchar(x/10%10+'0'); putchar(x%10+'0');}

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

void seek(uint32_t offset) {
    syscall(SYSCALL_SEEK, offset, 0, 0, 0);
}

char getchar() {
    uint8_t scancode = syscall(SYSCALL_GETCHAR, 0, 0, 0, 0);
    
    while (!scancode || scancode >= sizeof(scancode_ascii) || scancode_ascii[scancode] == '?') {
        if (scancode == BACKSPACE_SC) {
            return scancode;
        }
        
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

    char input[BUFFER_LEN] = { 0 };
    char* inputPtr = input;
    char tmp = 0;

    for (int i = 0; i < BUFFER_LEN && tmp != '\n'; i++) {
        tmp = getchar();
        if (tmp == BACKSPACE_SC) {
            if (i > 0) {
                i -= 2;
                tmp = 0;
                seek(-1);
                putchar(tmp);
                seek(-1);
            } else {
                i = -1;
            }
        }
        else {
            putchar(tmp);
            input[i] = tmp;
        }
    }

    while (*format != '\0') {
        if(*format == PRINTF_SPECIFIER) {
            format++;
            // I need to know whether the string is dynamic or not and what's its length.
            // when you use %s, you need to specify the length of the string before s or write -1 for it to be dynamic.
            int strLength = stoi(format);
            strLength = strLength < BUFFER_LEN ? strLength : BUFFER_LEN;
            for (; strLength && *format != 's'; format++); // skip the strlength after you know it's a string.
            
            switch (*format) {
            case PRINTF_SPECIFIER_CHAR:
                (**(char**)pArgument) = *inputPtr;
                inputPtr++;
                pArgument++;
                break;

            case PRINTF_SPECIFIER_DECIMAL:
                **pArgument = stoi(inputPtr);
                while (((*inputPtr >= '0' && *inputPtr <= '9') || *inputPtr == '-')) {
                    inputPtr++;
                }
                pArgument++;
                break;
            
            //4 different hex types
            case PRINTF_SPECIFIER_HEXADECIMAL_LOWERCASE:
            case PRINTF_SPECIFIER_HEXADECIMAL_UPPERCASE:
            case PRINTF_SPECIFIER_HEXADECIMAL_LOWERCASE_NOPREFIX:
            case PRINTF_SPECIFIER_HEXADECIMAL_UPPERCASE_NOPREFIX:
                **pArgument = stoh(inputPtr);
                while (((*inputPtr >= '0' && *inputPtr <= '9') || *inputPtr == '-')) {
                    inputPtr++;
                }
                pArgument++;
                break;

            case PRINTF_SPECIFIER_STRING:
                if (strLength == 0) // which means the string is dynamically allocated
                    memcpy(inputPtr, *pArgument, strlen(inputPtr));
                else
                    memcpy(inputPtr, *pArgument, strLength);
                inputPtr += strLength;
                pArgument++;
                break;

            case PRINTF_SPECIFIER:
            default:
                //no specifier, wait for a char that is in the format specified, which is '%'.
                while (*inputPtr != PRINTF_SPECIFIER) {
                    inputPtr++;
                }
                break;
            }
        }
        else{
            //no specifier, wait for a char that is in the format specified
            while (*inputPtr != *format) {
                inputPtr++;
            }
            inputPtr++;
        }
        format++;
    }
}
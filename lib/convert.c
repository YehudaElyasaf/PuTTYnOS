#include "convert.h"
#include "string.h"

#define BASE10 10
#define BASE16 0x10
#define LAST_DIGIT 9

#define PRINTN(x) {putchar(x/100%10+'0'); putchar(x/10%10+'0'); putchar(x%10+'0');}

void itoa(long n, char* buffer){
    int start = 0;
    
    if(n < 0){
        //the first char is '-'
        buffer[0] = '-';
        start = 1;
        n *= -1;
    }
    
    for(int i = start; n != 0; i++){
        buffer[i] = '0' + (n % BASE10);
        n /= BASE10;
    }

    if(buffer[start] == STRING_TERMINATOR){
        //n is zero
        buffer[start] = '0';
        buffer[start + 1] = STRING_TERMINATOR;
    }

    strrev(&buffer[start]);
}

#include "../kernel/io/print.h"
void itoh(unsigned long n, char* buffer){
    int start = 0;

    //write 0x
    buffer[start] = '0';
    start++;
    buffer[1] = 'x';
    start++;

    for(int i = start; n != 0; i++){
        int digit = n % BASE16;

        if(digit <= LAST_DIGIT)
            //digit is 0-9
            buffer[i] = '0' + digit;
        else
            //digit is A-F
            buffer[i] = 'A' - BASE10 + digit;
        
        n /= BASE16;
    }

    if(buffer[start] == STRING_TERMINATOR){
        buffer[start] = '0';
        buffer[start + 1] = STRING_TERMINATOR;
    }

    strrev(&buffer[start]); //reverse string from start
}

char between(int i, int a, int b) {
    return i >= a && i <= b;
}

int stoh(char* buffer) {
    int out = 0;
    if (buffer[0] == '0' && buffer[1] == 'x')
        buffer += 2;
    
    for (; between(*buffer, '0', '9') || between(*buffer, 'a', 'f') || between(*buffer, 'A', 'F'); buffer++) {
        out *= 0x10;
        if (between(*buffer, '0', '9')) {
            out += *buffer - '0';
        }
        else if (between(*buffer, 'a', 'f')) {
            out += *buffer - 'a' + 10;
        }
        else {
            out += *buffer - 'A' + 10;
        }
    }
    return out;
}

int stoi(char* buffer) {
    int out = 0, sign = 1;
    if (*buffer == '-') {
        sign = -1;
    }
    for (; between(*buffer, '0', '9'); buffer++) {
        out *= BASE10;
        out += *buffer - '0';
    }
    return out * sign;
}
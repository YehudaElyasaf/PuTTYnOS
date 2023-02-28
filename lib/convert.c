#include "convert.h"
#include "string.h"
#include "../kernel/io/print.h"
#include"memory.h"

#define BASE10 10
#define BASE2 2
#define BASE16 0x10
#define LAST_DIGIT 9

#define PRINTN(x) {putchar(x/100%10+'0'); putchar(x/10%10+'0'); putchar(x%10+'0');}

static void basetoa(long n, char* buffer, int base){
    int start = 0;

    //reset buffer
    char* mov = buffer;
    while(*mov != '\0'){
        *mov = '\0';
        mov++;
    }
    
    if(n < 0){
        //the first char is '-'
        buffer[0] = '-';
        start = 1;
        n *= -1;
    }
    
    int i = start;
    for(i; n != 0; i++){
        buffer[i] = '0' + (n % base);
        n /= base;
    }
    buffer[i] = 0;

    if(buffer[start] == STRING_TERMINATOR){
        //n is zero
        buffer[start] = '0';
        buffer[start + 1] = STRING_TERMINATOR;
    }

    strrev(&buffer[start]);
}

void itoa(long n, char* buffer){
    basetoa(n, buffer, BASE10);
}

void itoh(unsigned long n, char* buffer){
    int i;
    for(i = 0; n != 0; i++){
        int digit = n % BASE16;

        if(digit <= LAST_DIGIT)
            //digit is 0-9
            buffer[i] = '0' + digit;
        else
            //digit is A-F
            buffer[i] = 'A' - BASE10 + digit;
        
        n /= BASE16;
    }

    buffer[i] = '\0';
    if(buffer[0] == '\0'){
        buffer[0] = '0';
        buffer[1] = '\0';
    }


    strrev(buffer); //reverse string from start
}

void itob(unsigned long n, char* buffer){
    basetoa(n, buffer, BASE2);
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

void IPv4tos(uint8_t IPv4[IPv4_LENGTH], char* buffer){
    int offset = 0;

    for(int i = 0; i < IPv4_LENGTH; i++){
        itoa(IPv4[i], buffer + offset);
        offset = strlen(buffer);

        if(i < IPv4_LENGTH - 1){
            buffer[offset] = '.';
            offset++;
            buffer[offset] = '\0';
        }
    }
}

void MACtos(uint8_t MAC[MAC_LENGTH], char* buffer){
    int offset = 0;

    if(MAC[0] < 0x10){
        //first field has only one digit
        buffer[0] = '0';
        offset++;
    }

    for(int i = 0; i < MAC_LENGTH; i++){
        itoh(MAC[i], buffer + offset);
        offset = strlen(buffer);

        if(i < MAC_LENGTH - 1){
            buffer[offset] = ':';
            offset++;
            buffer[offset] = '\0';
            if(MAC[i + 1] < 0x10){
                buffer[offset] = '0';
                offset++;
                buffer[offset] = '\0';
            }
        }
    }
}

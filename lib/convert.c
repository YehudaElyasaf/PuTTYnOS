#include "convert.h"
#include "string.h"

#define BASE10 10
#define BASE16 0x10
#define LAST_DIGIT 9

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

    if(buffer[0] == 0)
        //n is zero
        buffer[0] = '0';

    strrev(&buffer[start]);
}

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
            buffer[i] = 'A' + digit;
        
        n /= BASE16;
    }

    strrev(&buffer[start]); //reverse string from start
}

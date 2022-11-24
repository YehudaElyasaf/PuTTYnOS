#include "convert.h"

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
    n = reverseNum(n, BASE10);
    
    for(int i = start; n != 0; i++){
        buffer[i] = '0' + (n % BASE10);
        n /= BASE10;
    }

    if(buffer[0] == 0)
        //n is zero
        buffer[0] = '0';
}

void itoh(unsigned long n, char* buffer){
    int start = 0;

    //write 0x
    buffer[start] = '0';
    start++;
    buffer[1] = 'x';
    start++;

    n = reverseNum(n, BASE16);
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

}

long reverseNum(long n, int base){
    int result = 0;
    
    for(int i = 0; n != 0; i++){
        result += n % base;
        n /= base;

        if(n != 0)
            result *= base;
    }

    return result;
}

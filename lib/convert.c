#include "convert.h"

#define BASE10 10
#define BASE16 0x10

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
        buffer[i] = '0' + (n % 10);
        n /= 10;
    }

    if(buffer[0] == 0)
        //n is zero
        buffer[0] = '0';
}

void itoh(long n, char* buffer){

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

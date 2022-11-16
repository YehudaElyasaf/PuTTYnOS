#include "PuTTYn.h"
#include "PagingHandler.h"

void initialize(){
    initPaging(0x0FF00000);
}

void main(){

    *((char*)0x00000000) = '0';

    char* video_memory = (char*) 0x000b8000;

    *(video_memory + 4) = *((char*)0x00000000);

    initialize();
    
    *(video_memory + 8) = *((char*)0x00000000);
    
}

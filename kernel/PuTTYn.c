#include "PuTTYn.h"
#include "io/output.h"
#include "memory/pagingHandler.h"
#include "idt.h"

void initialize(){
    initPaging(0x0FF00000);
}

void main(){

    *((char*)0x00001000) = '0';

    char* video_memory = (char*) 0x000b8000;

    *(video_memory + 0) = *((char*)0x00001000);

    initialize();
    
    *(video_memory + 2) = *((char*)0x00001000);
    
}

#include "PuTTYn.h"
#include "io/output.h"
#include "PagingHandler.h"
#include "idt.h"

void initialize(){
    initPaging(0x00000000);
    initIdt();
}

void main(){

    char* video_memory = (char*) 0xb8000;

    *(video_memory + 4) = *((char*)0xffff0000);

    initialize();
}

#include "PuTTYn.h"
#include "PagingHandler.h"

void initialize(){
    initPaging(0x00000000);
}

void main(){

    char* video_memory = (char*) 0xb8000;

    *(video_memory + 0) = *((char*)0xffff0000);

    initialize();
    
    *(video_memory + 2) = *((char*)0xffff0000);

}

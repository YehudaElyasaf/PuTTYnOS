#include "PuTTYn.h"

void initialize(){

}

void main(){
    initialize();
    
    char* video_memory = (char*) 0xb8000;
    *(video_memory + 0) = ' ';
    *(video_memory + 2) = '4';
    *(video_memory + 4) = '2';
    *(video_memory + 6) = ' ';
}

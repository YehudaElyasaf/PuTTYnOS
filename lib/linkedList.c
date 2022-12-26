#include "linkedList.h"
#include "printf.h"


void insertNode(uint32_t* head, uint32_t ptr, uint32_t nextOffset) {
    if (*head == LLEMPTY || *head > ptr) {
            *(uint32_t*)(ptr + nextOffset) = *head; // this is not working for some reason. something with pointers and stuff
            *head = ptr;
            return;
    }

    while (*head < ptr) {
        if (*((uint32_t*)(*head + nextOffset)) > ptr || *((uint32_t*)(*head + nextOffset)) == LLEMPTY) {
            *((uint32_t*)(ptr + nextOffset)) = *((uint32_t*)(*head + nextOffset));
            *((uint32_t*)(*head + nextOffset)) = ptr;
            return;
        }
        *head = *((uint32_t*)(*head + nextOffset));
    }
    printf("ERROR! linked list couldnt place a new node\n");
}

void removeNode(uint32_t* head, uint32_t ptr, uint32_t nextOffset) {
    if (*head == LLEMPTY) {
        printf("ERROR! linked list is Empty\n");
        return;
    }
    
    if (*head == ptr) {
        *head = LLEMPTY;
        return;
    }

    while (*head < ptr) {
        if (*((uint32_t*)(*head + nextOffset)) == ptr) {
            if (*((uint32_t*)(ptr + nextOffset)) == 0) {
                *((uint32_t*)(*head + nextOffset)) = -1;
                return;
            }
            *((uint32_t*)(*head + nextOffset)) = *((uint32_t*)(ptr + nextOffset));
            return;
        }
        *head = *((uint32_t*)(*head + nextOffset));
    }
    printf("Error! node is not in list\n");
}

uint32_t nodeByIndex(uint32_t head, uint32_t index, uint32_t nextOffset) {    
    if ((int32_t)index < 0)
        return LLEMPTY;
    
    for (int i = 0; i < index && head != LLEMPTY; i++) {
        head = *((uint32_t*)(nextOffset + head));
    }

    return head;
}

int placeInList(uint32_t head, uint32_t ptr, uint32_t nextOffset) {
    int i = 0;
    if (head == LLEMPTY)
        return LLEMPTY;
    while (head < ptr) {
        head = *((uint32_t*)(nextOffset + head));
        i++;
    }

    return head == ptr ? i : LLEMPTY;
}
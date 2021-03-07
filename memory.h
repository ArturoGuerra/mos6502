#ifndef _MEMORY_H_
#define _MEMORY_H_
#define MAX_MEN 1024 * 64;

typedef unsigned char Byte;
Byte Memory[MAX_MEM];

void init_memory(Byte memory[], int size) {
    for (int i = 0; size > i; i++) {
        memory[i] = 0;
    }
}

#endif
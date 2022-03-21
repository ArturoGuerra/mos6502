#include "m6502.h"
#include "m6502_instructions.h"
#include "memory.h"
#include "stdio.h"
#include "stdlib.h"

#define CONSOLE // will print out console messages from the cpu emulator

int main(int argc, char* argv[]) {
    //m6502_t cpu;

    if (argc != 2) {
        printf("Use ./m6502 <romfile>\n");
        return 1;
    }

    char* filename = argv[1];
    // CPU and Memory reset

    init_memory(Memory, MAX_MEM);

    FILE *fp;
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error opening file: %s\n", filename);
        return 1;
    }

    int size = fread(&Memory, sizeof(Byte), MAX_MEM, fp);
    fclose(fp);

    printf("Rom Size: %d\n", size);

    //init_m6502(&cpu);

    //printf("PC:0x%04X SP:0x%hhX A:0x%hhX X:0x%hhX Y:0x%hhX DB:0x%02X AB:0x%04X\n", cpu.PC, cpu.SP, cpu.A, cpu.X, cpu.Y, cpu.DB, cpu.AB);

    int running = 1;
    //int tick = 0;
    while(running) {
    }    


    printf("ZeroPage Mode 0x001F: %02X\n", Memory[0x001F]);
    printf("Absolute Mode 0x1FFF: %02X\n", Memory[0x1FFF]);
}

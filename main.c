#include "m6502.h"
#include "memory.h"
#include "stdio.h"
#include "stdlib.h"

int main(int argc, char* argv[]) {
    m6502_t cpu;

    if (argc != 3) {
        printf("Use ./m6502 <romfile> <cycle count>\n");
        return 1;
    }

    char* filename = argv[1];
    int inscount = atoi(argv[2]);
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


    
    init_m6502(0xFFFC, &cpu);

    // Need to make a reset sequence at some point
    cpu.DB = Memory[cpu.PC];
    
    printf("PC:0x%04X SP:0x%hhX A:0x%hhX X:0x%hhX Y:0x%hhX DB:0x%02X AB:0x%04X\n", cpu.PC, cpu.SP, cpu.A, cpu.X, cpu.Y, cpu.DB, cpu.AB);
    int ticks = 7 + inscount;
    for(int i = 1; ticks >= i; i++) {
        tick_m6502(&cpu);
        printf("Tick:%d PC:0x%04X SP:0x%hhX A:0x%hhX X:0x%hhX Y:0x%hhX DB:0x%02X AB:0x%04X INS:0x%02X\n", i, cpu.PC, cpu.SP, cpu.A, cpu.X, cpu.Y, cpu.DB, cpu.AB, cpu.INS);

        // Read
        if (cpu.RW) {
            cpu.DB = Memory[cpu.AB];
            printf("Reading from:0x%04X Data:0x%02X\n", cpu.AB, cpu.DB);
        }
        
        // Write
        else {
            printf("Writing to: %04X\n", cpu.AB);
            Memory[cpu.AB] = cpu.DB;
        }
        
    }
    

//    int i = 1;
//
//    while(cpu.RDY) {
//        printf("Tick: %d\n", i);
//        tick_m6502(&cpu, Memory);
//        printf("PC:0x%04X SP:0x%hhX A:0x%hhX X:0x%hhX Y:0x%hhX\n", cpu.PC, cpu.SP, cpu.A, cpu.X, cpu.Y);
//        i++;
//    }
}
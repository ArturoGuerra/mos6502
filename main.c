#include "m6502.h"
#include "memory.h"
#include "stdio.h"
#include "stdlib.h"


int main(int argc, char* argv[]) {
    m6502_t cpu;

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
    
    init_m6502(&cpu);

    printf("PC:0x%04X SP:0x%hhX A:0x%hhX X:0x%hhX Y:0x%hhX DB:0x%02X AB:0x%04X\n", cpu.PC, cpu.SP, cpu.A, cpu.X, cpu.Y, cpu.DB, cpu.AB);

    int running = 1;
    int tick = 0;
    while(running) {
        tick_m6502(&cpu);
        //if (tick > 6) printf("Tick:%d PC:0x%04X SP:0x%hhX A:0x%hhX X:0x%hhX Y:0x%hhX DB:0x%02X AB:0x%04X R/W:%d\n", tick, cpu.PC, cpu.SP, cpu.A, cpu.X, cpu.Y, cpu.DB, cpu.AB, cpu.RW);
        printf("Tick:%d PC:0x%04X SP:0x%hhX A:0x%hhX X:0x%hhX Y:0x%hhX DB:0x%02X AB:0x%04X R/W:%d\n", tick, cpu.PC, cpu.SP, cpu.A, cpu.X, cpu.Y, cpu.DB, cpu.AB, cpu.RW);
        if (tick > 6 && cpu.IR == INS_BRK_IMP << 3) break;


        // Read
        if (cpu.RW) {
            cpu.DB = Memory[cpu.AB];
        }
        
        // Write
        else {
            Memory[cpu.AB] = cpu.DB;
        }

        tick++;
    }    


    printf("ZeroPage Mode 0x001F: %02X\n", Memory[0x001F]);
    printf("Absolute Mode 0x1FFF: %02X\n", Memory[0x1FFF]);
}
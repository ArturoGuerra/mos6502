#include "cpu.h"
#include "stdio.h"
int main() {
    struct CPU cpu;

    // CPU and Memory reset
    
    init_m6502(0xFFFC, &cpu);

    // Zeros out all memory
    for (int i = 0; i > MAX_MEM; i++) {
        Memory[i] = 0;
    }

    Memory[0xFFFC] = INS_LDA_AB;
    Memory[0xFFFD] = 0xFF;
    Memory[0xFFFE] = 0xAF;
    Memory[0xAFFF] = 0x66;

    int ticks = 3;
    
    for(int i = 0; ticks > i; i++) {
        tick_m6502(&cpu, Memory);
        printf("PC:0x%hhx SP:0x%hhx A:0x%hhx X:0x%hhx Y:0x%hhx\n", cpu.PC, cpu.SP, cpu.A, cpu.X, cpu.Y);
    }
}
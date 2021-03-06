#include "m6502.h"
#include "stdio.h"
int main() {
    m6502 cpu;

    // CPU and Memory reset
    
    init_m6502(0xFFFC, &cpu);
    init_memory(Memory, MAX_MEM);    
    
    // 4 Cycles 3 Bytes
    Memory[0xFFFC] = INS_LDA_AB; // PC 
    Memory[0xFFFD] = 0xFF; // PC
    Memory[0xFFFE] = 0xAF; // PC
    Memory[0xAFFF] = 0x43;
    
    // 2 Cycles 2 Bytes
    Memory[0xFFFF] = INS_LDX_IM; // PC
    Memory[0x0000] = 0x10; // PC

    // 4 Cycles 2 Bytes
    Memory[0x0001] = INS_LDA_ZPX; // PC
    Memory[0x0002] = 0x67; // PC
    Memory[0x0077] = 0x42;


    int ticks = 10;
    
    for(int i = 1; ticks >= i; i++) {
        printf("Tick: %d\n", i);
        tick_m6502(&cpu, Memory);
        printf("PC:0x%04X SP:0x%hhX A:0x%hhX X:0x%hhX Y:0x%hhX\n", cpu.PC, cpu.SP, cpu.A, cpu.X, cpu.Y);
    }
}
#include "m6502.h"
#include "memory.h"
#include "stdio.h"

int main() {
    m6502_t cpu;

    // CPU and Memory reset
    
    init_memory(Memory, MAX_MEM);    

/* Working set of instructions */    
//    // 3 Cycles 2 Bytes
//    Memory[0xFFFC] = INS_LDA_ZP; // PC
//    Memory[0xFFFD] = 0xA1; // PC
//    Memory[0x00A1] = 0x22; // A register should be 0x22
//
//
//    // 2 Cycles 2 Bytes
//    Memory[0xFFFE] = INS_LDX_IM; // PC
//    Memory[0xFFFF] = 0x10; // PC X register should be 0x10
//    
//    // 4 Cycles 2 Bytes
//    Memory[0x0000] = INS_LDA_ZPX; // PC
//    Memory[0x0001] = 0x67; // PC
//    Memory[0x0077] = 0x42; // X register should be 0x42
//
//    // 2 Cycles 2 Bytes
//    Memory[0x0002] = INS_LDA_IM; // PC
//    Memory[0x0003] = 0x69; // PC A 0x69


    // 4 Cycles 3 Bytes
    Memory[0xFFFC] = INS_LDA_AB; // PC
    Memory[0xFFFD] = 0xFF; // PC
    Memory[0xFFFE] = 0xAF; // PC
    Memory[0xAFFF] = 0x43;

    Memory[0xFFFF] = INS_LDA_IM;
    Memory[0x0000] = 0x30;

    
    init_m6502(0xFFFC, &cpu);

    // Need to make a reset sequence at some point
    cpu.DB = Memory[cpu.PC];

    int ticks = 4 + 2;
    for(int i = 1; ticks >= i; i++) {
        printf("Tick:%d PC:0x%04X SP:0x%hhX A:0x%hhX X:0x%hhX Y:0x%hhX DataBus:0x%02X AddrBus:0x%04X\n", i, cpu.PC, cpu.SP, cpu.A, cpu.X, cpu.Y, cpu.DB, cpu.AB);
        tick_m6502(&cpu);

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
    
    printf("PC:0x%04X SP:0x%hhX A:0x%hhX X:0x%hhX Y:0x%hhX DataBus:0x%02X AddrBus:0x%04X\n", cpu.PC, cpu.SP, cpu.A, cpu.X, cpu.Y, cpu.DB, cpu.AB);

//    int i = 1;
//
//    while(cpu.RDY) {
//        printf("Tick: %d\n", i);
//        tick_m6502(&cpu, Memory);
//        printf("PC:0x%04X SP:0x%hhX A:0x%hhX X:0x%hhX Y:0x%hhX\n", cpu.PC, cpu.SP, cpu.A, cpu.X, cpu.Y);
//        i++;
//    }
}
#include <stdio.h>
#include <assert.h>
#include "cpu.h"
void _SYNC_ON(struct CPU *cpu) {
    cpu->SYNC = 1;
    cpu->buffer = 0;
}

void _SYNC_OFF(struct CPU *cpu) {
    cpu->SYNC = 0;
    cpu->buffer = 0;
}

Byte fetchByte(struct CPU *cpu, Byte memory[]) {
    Byte data = memory[cpu->PC];
    cpu->PC++;
    return data;
}

Byte fetchZP(struct CPU *cpu, Byte memory[]) {
    // Makes sure its in the zero page range
    assert(cpu->PC >= 0x0000 && cpu->PC <= 0x00FF);
    return fetchByte(cpu, memory);
}

void SetNegativeAndZeroFlags(struct CPU *cpu, Byte value) {
    cpu->Flags.Z = value == 0;
    cpu->Flags.N = (value & 0b10000000) > 0;
}

void init_m6502(Word initVector, struct CPU *cpu) {
    // Sets the program counter vector
    cpu->PC = initVector;
    // Sets the stack pointer to 255
    cpu->SP = 0xFF;
    // Sets all cpu flags to 0
    cpu->Flags.C = cpu->Flags.Z = cpu->Flags.V = cpu->Flags.N = cpu->Flags.I = cpu->Flags.D = cpu->Flags.B = 0;
    // Sets all cpu registers to 0
    cpu->A = cpu->X = cpu->Y = 0;

    cpu->SYNC = 1;
    cpu->buffer = 0;
    cpu->IR = (cpu->PC << 3);
}

void tick_m6502(struct CPU* cpu, Byte memory[]) {
    if (cpu->SYNC) {
        _SYNC_OFF(cpu);
        cpu->IR = memory[cpu->PC] << 3;
        cpu->PC++;
    }

    switch(cpu->IR++) {
    case INS_LDA_IM<<3|0: cpu->A = fetchByte(cpu, memory);SetNegativeAndZeroFlags(cpu, cpu->A);_SYNC_ON(cpu);break;

    case INS_LDA_ZP<<3|0: cpu->buffer = fetchZP(cpu, memory); break;
    case INS_LDA_ZP<<3|1: cpu->A = memory[cpu->buffer];SetNegativeAndZeroFlags(cpu, cpu->A);break;

    case INS_LDA_ZPX<<3|0: cpu->buffer = fetchZP(cpu, memory); break;
    case INS_LDA_ZPX<<3|1:cpu->A = memory[cpu->PC];SetNegativeAndZeroFlags(cpu, cpu->A);break;
    case INS_LDA_ZPX<<3|2: cpu->A += cpu->X;SetNegativeAndZeroFlags(cpu, cpu->A);_SYNC_OFF(cpu);break;

    case INS_LDA_AB<<3|0: cpu->buffer = fetchByte(cpu, memory); break;
    case INS_LDA_AB<<3|1: cpu->buffer |= (Word)fetchByte(cpu, memory) << 8; break;
    case INS_LDA_AB<<3|2:cpu->A = memory[cpu->buffer];SetNegativeAndZeroFlags(cpu, cpu->A);_SYNC_OFF(cpu);break;

    case INS_LDA_ABX<<3|0: break;


    case INS_NOP_IMP: break;
    default:
        printf("Unknown instruction\n");
        break;
    }    
}
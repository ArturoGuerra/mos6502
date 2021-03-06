#include <stdio.h>
#include <assert.h>
#include "m6502.h"

void init_memory(Byte memory[], int size) {
    for (int i = 0; size > i; i++) {
        memory[i] = 0;
    }
}

void _SYNC_ON(m6502 *cpu) {
    cpu->SYNC = 1;
    cpu->buffer = 0;
}

void _SYNC_OFF(m6502 *cpu) {
    cpu->SYNC = 0;
    cpu->buffer = 0;
}

Byte fetchByte(m6502 *cpu, Byte memory[]) {
    Byte data = memory[cpu->PC];
    cpu->PC++;
    return data;
}

Byte fetchZP(m6502*cpu, Byte memory[]) {
    // Makes sure its in the zero page range
    assert(cpu->PC >= 0x0000 && cpu->PC <= 0x00FF);
    return fetchByte(cpu, memory);
}

void SetNegativeAndZeroFlags(m6502 *cpu, Byte value) {
    cpu->Flags.Z = value == 0;
    cpu->Flags.N = (value & 0b10000000) > 0;
}

void init_m6502(Word initVector, m6502 *cpu) {
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

void tick_m6502(m6502 *cpu, Byte memory[]) {
    if (cpu->SYNC) {
        _SYNC_OFF(cpu);
        cpu->Instruction = memory[cpu->PC];
        cpu->IR = memory[cpu->PC] << 3;
        cpu->PC++;
        return;
    }

    switch(cpu->IR++) {
    case INS_LDA_IM<<3|0: cpu->A = fetchByte(cpu, memory);SetNegativeAndZeroFlags(cpu, cpu->A);_SYNC_ON(cpu);break;

    case INS_LDA_ZP<<3|0: cpu->buffer = fetchZP(cpu, memory); break;
    case INS_LDA_ZP<<3|1: cpu->A = memory[cpu->buffer];SetNegativeAndZeroFlags(cpu, cpu->A);_SYNC_ON(cpu);break;

    case INS_LDA_ZPX<<3|0: cpu->buffer = fetchZP(cpu, memory); break;
    case INS_LDA_ZPX<<3|1: cpu->buffer += cpu->X;break;
    case INS_LDA_ZPX<<3|2: cpu->A = memory[cpu->buffer];SetNegativeAndZeroFlags(cpu, cpu->A);_SYNC_ON(cpu);break;

    case INS_LDA_AB<<3|0: cpu->buffer = fetchByte(cpu, memory); break;
    case INS_LDA_AB<<3|1: cpu->buffer |= (Word)fetchByte(cpu, memory) << 8; break;
    case INS_LDA_AB<<3|2: cpu->A = memory[cpu->buffer];SetNegativeAndZeroFlags(cpu, cpu->A);_SYNC_ON(cpu);break;

    // Has between 4 and 5 cycles
    case INS_LDA_ABX<<3|0: cpu->buffer = fetchByte(cpu, memory); break;
    case INS_LDA_ABX<<3|1: cpu->buffer |= (Word)fetchByte(cpu, memory) << 8; break;
    case INS_LDA_ABX<<3|2: break;
    case INS_LDA_ABX<<3|3: break;

    case INS_LDA_ABY<<3|0: break;
    case INS_LDA_ABY<<3|1: break;
    case INS_LDA_ABY<<3|2: break;
    case INS_LDA_ABY<<3|3: break;

    case INS_LDA_INX<<3|0: break;
    case INS_LDA_INX<<3|1: break;
    case INS_LDA_INX<<3|2: break;
    case INS_LDA_INX<<3|3: break;
    case INS_LDA_INX<<3|4: break;

    case INS_LDA_INY<<3|0: break;
    case INS_LDA_INY<<3|1: break;
    case INS_LDA_INY<<3|2: break;
    case INS_LDA_INY<<3|3: break;
    case INS_LDA_INY<<3|4: break;

    case INS_LDX_IM<<3|0: cpu->X = fetchByte(cpu, memory); SetNegativeAndZeroFlags(cpu, cpu->X); _SYNC_ON(cpu); break;

    case INS_LDX_ZP<<3|0: cpu->buffer = fetchZP(cpu, memory); break;
    case INS_LDX_ZP<<3|1: cpu->X = memory[cpu->buffer]; SetNegativeAndZeroFlags(cpu, cpu->X); _SYNC_ON(cpu); break;

    case INS_LDX_ZPY<<3|0: cpu->buffer = fetchZP(cpu, memory); break;
    case INS_LDX_ZPY<<3|1: cpu->buffer += cpu->Y; break;
    case INS_LDX_ZPY<<3|2: cpu->X = memory[cpu->buffer]; SetNegativeAndZeroFlags(cpu, cpu->X); _SYNC_ON(cpu); break;

    case INS_LDX_AB<<3|0: cpu->buffer = fetchByte(cpu, memory); break;
    case INS_LDX_AB<<3|1: cpu->buffer |= (Word)fetchByte(cpu, memory) << 8; break;
    case INS_LDX_AB<<3|2: cpu->X = memory[cpu->buffer]; SetNegativeAndZeroFlags(cpu, cpu->X); _SYNC_ON(cpu); break;

    case INS_LDX_ABY<<3|0: break;
    case INS_LDX_ABY<<3|1: break;
    case INS_LDX_ABY<<3|2: break;
    case INS_LDX_ABY<<3|3: break;



    case INS_NOP_IMP<<3|0: _SYNC_ON(cpu); break;
    default:
        printf("Unknown instruction: %04X\n", cpu->Instruction);
        break;
    }    
}
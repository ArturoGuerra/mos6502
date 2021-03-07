#include <stdio.h>
#include <assert.h>
#include "m6502.h"

#define FB() cpu->AB = cpu->PC;
#define FBZ() cpu->AB = cpu->DB;
#define FBX() cpu->AB += cpu->X;
#define FBY() cpu->AB += cpu->Y;
#define FBIRX() cpu->AB = cpu->IRX;
#define PC() cpu->PC++;
#define SA(addr) cpu->AB = addr;
#define _SYNC_ON() cpu->SYNC = 1; cpu->AB = cpu->PC;
#define _SYNC_OFF() cpu->SYNC = 0;

void SetNegativeAndZeroFlags(m6502_t *cpu, Byte value) {
    cpu->Flags.Z = value == 0;
    cpu->Flags.N = (value & 0b10000000) > 0;
}

void init_m6502(Word initVector, m6502_t *cpu) {
    // Sets the program counter vector
    cpu->PC = initVector;
    // Sets the stack pointer to 255
    cpu->SP = 0xFF;
    // Sets all cpu flags to 0
    cpu->Flags.C = cpu->Flags.Z = cpu->Flags.V = cpu->Flags.N = cpu->Flags.I = cpu->Flags.D = cpu->Flags.B = 0;
    // Sets all cpu registers to 0
    cpu->A = cpu->X = cpu->Y = 0;

    cpu->SYNC = 1;
    cpu->RDY = 1;
    cpu->RW = 1;
    cpu->IR = (cpu->PC << 3);
    cpu->AB = cpu->PC;
}


void tick_m6502(m6502_t *cpu) {
    if (cpu->SYNC) {
        _SYNC_OFF();
        //printf("Instruction: %02X\n", cpu->DataBus);
        cpu->INS = cpu->DB;
        cpu->IR = cpu->DB << 3;
        cpu->RW = 1;
        cpu->IRX = cpu->IRY = 0;
        FB();
        PC();
        SA(cpu->PC);
        return;
    }

    switch(cpu->IR++) {
    case INS_LDA_IM<<3|0:cpu->A = cpu->DB;PC();SetNegativeAndZeroFlags(cpu, cpu->A);_SYNC_ON();break;

    case INS_LDA_ZP<<3|0:FBZ();PC();break;
    case INS_LDA_ZP<<3|1:cpu->A = cpu->DB;SetNegativeAndZeroFlags(cpu, cpu->A);_SYNC_ON();break;

    case INS_LDA_ZPX<<3|0:FBZ();PC();break;
    case INS_LDA_ZPX<<3|1:FBX();break;
    case INS_LDA_ZPX<<3|2:cpu->A = cpu->DB;SetNegativeAndZeroFlags(cpu, cpu->A);_SYNC_ON();break;

    case INS_LDA_AB<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_LDA_AB<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();FBIRX();break;
    case INS_LDA_AB<<3|2:cpu->A = cpu->DB;SetNegativeAndZeroFlags(cpu, cpu->A);_SYNC_ON();break;

//    // Has between 4 and 5 cycles
    case INS_LDA_ABX<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_LDA_ABX<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();cpu->AB = cpu->IRX + cpu->X;break;
    case INS_LDA_ABX<<3|2:if (!(cpu->AB ^ cpu->IRX) >> 8) cpu->A = cpu->DB;_SYNC_ON();break;
    case INS_LDA_ABX<<3|3:cpu->A = cpu->DB;_SYNC_ON();break;

//    // Has between 4 and 5 cycles
    case INS_LDA_ABY<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_LDA_ABY<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();cpu->AB = cpu->IRX + cpu->Y;break;
    case INS_LDA_ABY<<3|2:if (!(cpu->AB ^ cpu->IRX) >> 8) cpu->A = cpu->DB;_SYNC_ON();break;
    case INS_LDA_ABY<<3|3:cpu->A = cpu->DB;_SYNC_ON();break;

    case INS_LDA_INX<<3|0: break;
    case INS_LDA_INX<<3|1: break;
    case INS_LDA_INX<<3|2: break;
    case INS_LDA_INX<<3|3: break;
    case INS_LDA_INX<<3|4: _SYNC_ON();break;

    case INS_LDA_INY<<3|0: break;
    case INS_LDA_INY<<3|1: break;
    case INS_LDA_INY<<3|2: break;
    case INS_LDA_INY<<3|3: 
        _SYNC_ON();
        break;
    case INS_LDA_INY<<3|4:
        _SYNC_ON();
        break;

    case INS_LDX_IM<<3|0:cpu->X = cpu->DB;PC();SetNegativeAndZeroFlags(cpu, cpu->X);_SYNC_ON();break;
    
    case INS_LDX_ZP<<3|0:FBZ();PC();break;
    case INS_LDX_ZP<<3|1:cpu->X = cpu->DB;SetNegativeAndZeroFlags(cpu, cpu->X);_SYNC_ON();break;
    
    case INS_LDX_ZPY<<3|0:FBZ();PC();break;
    case INS_LDX_ZPY<<3|1:FBY();break;
    case INS_LDX_ZPY<<3|2:cpu->X = cpu->DB;SetNegativeAndZeroFlags(cpu, cpu->X);_SYNC_ON();break;
    
    case INS_LDX_AB<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_LDX_AB<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();FBIRX();break;
    case INS_LDX_AB<<3|2:cpu->X = cpu->DB;SetNegativeAndZeroFlags(cpu, cpu->X);_SYNC_ON();break;

//    case INS_LDX_ABY<<3|0: break;
//    case INS_LDX_ABY<<3|1: break;
//    case INS_LDX_ABY<<3|2: break;
//    case INS_LDX_ABY<<3|3: break;

    case INS_NOP_IMP<<3|0: _SYNC_ON(); break;
    default:
        printf("Unknown instruction:0x%02X\n", cpu->INS);
        break;
    }    
}
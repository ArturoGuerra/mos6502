#include <stdio.h>
#include <assert.h>
#include "m6502.h"

// Tells the cpu to exec instruction or fetch instruction
#define _SYNC_ON() cpu->SYNC = 1; cpu->AB = cpu->PC;
#define _SYNC_OFF() cpu->SYNC = 0;

#define FB() cpu->AB = cpu->PC;
#define FBZ() cpu->AB = cpu->DB;
#define FBX() cpu->AB += cpu->X;
#define FBY() cpu->AB += cpu->Y;
#define FBIRX() cpu->AB = cpu->IRX;
#define PC() cpu->PC++;
#define SAB(addr) cpu->AB = addr;

// CPU Status Flags
#define M6502_NF 0b10000000
#define M6502_VF 0b01000000
#define M6502_BF 0b00110000 // Not a real CPU Flag
#define M6502_DF 0b00001000
#define M6502_IF 0b00000100
#define M6502_ZF 0b00000010
#define M6502_CF 0b00000001

void set_nz(m6502_t *cpu, Byte value) {
    (value == 0) ? (cpu->Flags |= M6502_ZF) : (cpu->Flags &= ~M6502_ZF);
    ((value & 0b10000000) > 0) ? (cpu->Flags |= M6502_NF) : (cpu->Flags &= ~M6502_NF);
}

void init_m6502(Word initVector, m6502_t *cpu) {
    // Sets the program counter vector
    cpu->PC = initVector;
    // Sets the stack pointer to 255
    cpu->SP = 0x00;
    // Sets all cpu flags to 0
    cpu->Flags = 0;
    // Sets all cpu registers to 0
    cpu->A = cpu->X = cpu->Y = 0;

    // Sync pin is set to 1 so it can fetch an instruction
    cpu->SYNC = 1;
    // Ready pin is set to 1 after startup
    cpu->RDY = 0;
    // RW is set to 1 so it can read from the bus
    cpu->RW = 1;
    cpu->IR = (cpu->PC << 3);
    SAB(cpu->PC);
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
        SAB(cpu->PC);
        return;
    }

    switch(cpu->IR++) {
    case INS_LDA_IM<<3|0:cpu->A = cpu->DB;PC();set_nz(cpu, cpu->A);_SYNC_ON();break;

    case INS_LDA_ZP<<3|0:FBZ();PC();break;
    case INS_LDA_ZP<<3|1:cpu->A = cpu->DB;set_nz(cpu, cpu->A);_SYNC_ON();break;

    case INS_LDA_ZPX<<3|0:FBZ();PC();break;
    case INS_LDA_ZPX<<3|1:FBX();break;
    case INS_LDA_ZPX<<3|2:cpu->A = cpu->DB;set_nz(cpu, cpu->A);_SYNC_ON();break;

    case INS_LDA_AB<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_LDA_AB<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();FBIRX();break;
    case INS_LDA_AB<<3|2:cpu->A = cpu->DB;set_nz(cpu, cpu->A);_SYNC_ON();break;

    // Has between 4 and 5 cycles
    case INS_LDA_ABX<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_LDA_ABX<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();cpu->AB = cpu->IRX + cpu->X;break;
    case INS_LDA_ABX<<3|2:if (!(cpu->AB ^ cpu->IRX) >> 8) cpu->A = cpu->DB;_SYNC_ON();break;
    case INS_LDA_ABX<<3|3:cpu->A = cpu->DB;_SYNC_ON();break;

    // Has between 4 and 5 cycles
    case INS_LDA_ABY<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_LDA_ABY<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();cpu->AB = cpu->IRX + cpu->Y;break;
    case INS_LDA_ABY<<3|2:if (!(cpu->AB ^ cpu->IRX) >> 8) cpu->A = cpu->DB;_SYNC_ON();break;
    case INS_LDA_ABY<<3|3:cpu->A = cpu->DB;_SYNC_ON();break;

    case INS_LDA_INX<<3|0: 
        FBZ();
        PC();
        break;
    case INS_LDA_INX<<3|1: break;
        cpu->AB = cpu->DB + cpu->X;
        break;
    case INS_LDA_INX<<3|2: break;
        cpu->IRX = cpu->DB;PC();FB();
        break;
    case INS_LDA_INX<<3|3: break;
        cpu->IRX |= (Word)cpu->DB << 8;
        PC();
        FBIRX();
        break;
    case INS_LDA_INX<<3|4: _SYNC_ON();
        cpu->A = cpu->DB;set_nz(cpu, cpu->A);_SYNC_ON();
        break;

    case INS_LDA_INY<<3|0: break;
    case INS_LDA_INY<<3|1: break;
    case INS_LDA_INY<<3|2: break;
    case INS_LDA_INY<<3|3: 
        _SYNC_ON();
        break;
    case INS_LDA_INY<<3|4:
        _SYNC_ON();
        break;

    case INS_LDX_IM<<3|0:cpu->X = cpu->DB;PC();set_nz(cpu, cpu->X);_SYNC_ON();break;
    
    case INS_LDX_ZP<<3|0:FBZ();PC();break;
    case INS_LDX_ZP<<3|1:cpu->X = cpu->DB;set_nz(cpu, cpu->X);_SYNC_ON();break;
    
    case INS_LDX_ZPY<<3|0:FBZ();PC();break;
    case INS_LDX_ZPY<<3|1:FBY();break;
    case INS_LDX_ZPY<<3|2:cpu->X = cpu->DB;set_nz(cpu, cpu->X);_SYNC_ON();break;
    
    case INS_LDX_AB<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_LDX_AB<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();FBIRX();break;
    case INS_LDX_AB<<3|2:cpu->X = cpu->DB;set_nz(cpu, cpu->X);_SYNC_ON();break;

//    case INS_LDX_ABY<<3|0: break;
//    case INS_LDX_ABY<<3|1: break;
//    case INS_LDX_ABY<<3|2: break;
//    case INS_LDX_ABY<<3|3: break;

    case INS_NOP_IMP<<3|0: _SYNC_ON(); break;
    default:
        printf("Unknown instruction:0x%02X\n", cpu->INS);
        //getchar();
        break;
    }    
}
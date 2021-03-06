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
#define PAGECROSS(a1, a2) ((a1 >> 8) != (a2 >> 8))

// CPU Status Flags
#define M6502_NF 128 //0b10000000
#define M6502_VF 64 //0b01000000
#define M6502_BF 48 //0b00110000 // Not a real CPU Flag
#define M6502_DF 8 //0b00001000
#define M6502_IF 4 //0b00000100
#define M6502_ZF 2 //0b00000010
#define M6502_CF 1 //0b00000001

#define WRITE() cpu->RW = 0;
#define READ() cpu->RW = 1;

#define PUSH() cpu->SP--;
#define POP() cpu->SP++;
#define RWSTACK() cpu->AB = cpu->SP;

void set_nz(m6502_t *cpu, Byte value) {
    (value == 0) ? (cpu->P |= M6502_ZF) : (cpu->P &= ~M6502_ZF);
    ((value & 0b10000000) > 0) ? (cpu->P |= M6502_NF) : (cpu->P &= ~M6502_NF);
}

void set_carry(m6502_t *cpu, Byte value) {
    ((value & 0b10000000) > 0) ? (cpu->P |= M6502_CF) : (cpu->P &= ~M6502_CF);
}


void init_m6502(m6502_t *cpu) {
    // Sets the program counter vector
    cpu->PC = 0xFFFC;
    // Sets the stack pointer to 255
    cpu->SP = 0xFF;
    // Sets all cpu flags to 0
    cpu->P = 0;
    // Sets all cpu registers to 0
    cpu->A = cpu->X = cpu->Y = 0;

    // Sync pin is set to 1 so it can fetch an instruction
    cpu->SYNC = 1;
    // Ready pin is set to 1 after startup
    cpu->RDY = 1;
    // Forces the cpu into a reset state
    cpu->RESET = 1;
    // RW is set to 1 so it can read from the bus
    cpu->RW = 1;
    cpu->IR = (cpu->PC << 3);
    SAB(cpu->PC);
}

void tick_m6502(m6502_t *cpu) {
    if (cpu->SYNC) {
        _SYNC_OFF();
        //printf("Instruction: %02X\n", cpu->DataBus);
        cpu->IR = cpu->DB << 3;
        cpu->IRX = 0;
        if (cpu->RESET) {
            cpu->IR = INS_BRK_IMP << 3;
            cpu->RESET = 0;
        } else {
            PC();
            FB();
            printf("Loaded Instruction: 0x%02X\n", cpu->DB);
        }

        return;
    }

    READ();
    switch(cpu->IR++) {

    /* To be tested */
    case INS_AND_IM<<3|0:break;
    
    case INS_AND_ZP<<3|0:break;
    case INS_AND_ZP<<3|1:break;

    case INS_AND_ZPX<<3|0:break;
    case INS_AND_ZPX<<3|1:break;
    case INS_AND_ZPX<<3|2:break;

    case INS_AND_AB<<3|0:break;
    case INS_AND_AB<<3|1:break;
    case INS_AND_AB<<3|2:break;

    case INS_AND_ABX<<3|0:break;
    case INS_AND_ABX<<3|1:break;
    case INS_AND_ABX<<3|2:break;
    case INS_AND_ABX<<3|3:break;

    case INS_AND_ABY<<3|0:break;
    case INS_AND_ABY<<3|1:break;
    case INS_AND_ABY<<3|2:break;
    case INS_AND_ABY<<3|3:break;

    case INS_AND_INX<<3|0:break;
    case INS_AND_INX<<3|1:break;
    case INS_AND_INX<<3|2:break;
    case INS_AND_INX<<3|3:break;
    case INS_AND_INX<<3|4:break;

    case INS_AND_INY<<3|0:break;
    case INS_AND_INY<<3|1:break;
    case INS_AND_INY<<3|2:break;
    case INS_AND_INY<<3|3:break;
    case INS_AND_INY<<3|4:break;

    case INS_EOR_IM<<3|0:break;

    case INS_EOR_ZP<<3|0:break;
    case INS_EOR_ZP<<3|1:break;

    case INS_EOR_ZPX<<3|0:break;
    case INS_EOR_ZPX<<3|1:break;
    case INS_EOR_ZPX<<3|2:break;

    case INS_EOR_AB<<3|0:break;
    case INS_EOR_AB<<3|1:break;
    case INS_EOR_AB<<3|2:break;

    case INS_EOR_ABX<<3|0:break;
    case INS_EOR_ABX<<3|1:break;
    case INS_EOR_ABX<<3|2:break;
    /* ------------ */

    /* --- Tested Instructions --- */
    // First instruction that will be executed (Should be replaced by PHP smh)
    case INS_BRK_IMP<<3|0:PC();cpu->DB = cpu->PC >> 8;RWSTACK();WRITE();PUSH();break;
    case INS_BRK_IMP<<3|1:cpu->DB = cpu->PC & 0xFF;RWSTACK();WRITE();PUSH();break;
    case INS_BRK_IMP<<3|2:cpu->DB = cpu->P;RWSTACK();WRITE();PUSH();break;
    case INS_BRK_IMP<<3|3:cpu->AB = 0xFFFC;break;
    case INS_BRK_IMP<<3|4:cpu->PC = cpu->DB;cpu->AB++;break;
    case INS_BRK_IMP<<3|5:cpu->PC |= (Word)cpu->DB << 8;_SYNC_ON();break;
    
    case INS_ASL_ZP<<3|0:PC();FBZ();break;
    case INS_ASL_ZP<<3|1:set_carry(cpu, cpu->DB);cpu->IRX = cpu->DB << 1;set_nz(cpu, cpu->IRX);break;
    case INS_ASL_ZP<<3|2:cpu->DB = cpu->IRX;WRITE();break;
    case INS_ASL_ZP<<3|3:_SYNC_ON();break;
    
    // Execute first for best performance PHP is slow :) :)
    case INS_PHP_IMP<<3|0:cpu->DB = cpu->P;RWSTACK();WRITE();break;
    case INS_PHP_IMP<<3|1:PUSH();_SYNC_ON();break;
    
    case INS_ASL_ACC<<3|0:set_carry(cpu, cpu->A);cpu->A = cpu->A << 1;set_nz(cpu, cpu->A);_SYNC_ON();break;
    
    case INS_ASL_AB<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_ASL_AB<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();FBIRX();break;
    case INS_ASL_AB<<3|2:set_carry(cpu, cpu->DB);cpu->IRX = cpu->DB << 1;set_nz(cpu, cpu->IRX);break;
    case INS_ASL_AB<<3|3:cpu->DB = cpu->IRX;WRITE();break;
    case INS_ASL_AB<<3|4:_SYNC_ON();break;
    
    case INS_ASL_ZPX<<3|0:PC();FBZ();break;
    case INS_ASL_ZPX<<3|1:cpu->AB += cpu->X;break;
    case INS_ASL_ZPX<<3|2:set_carry(cpu, cpu->DB);cpu->IRX = cpu->DB << 1;set_nz(cpu, cpu->IRX);break;
    case INS_ASL_ZPX<<3|3:cpu->DB = cpu->IRX;WRITE();break;
    case INS_ASL_ZPX<<3|4:_SYNC_ON();break;
    
    case INS_ASL_ABX<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_ASL_ABX<<3|1:cpu->IRX |= cpu->DB << 8;PC();break;
    case INS_ASL_ABX<<3|2:cpu->IRX += cpu->X;FBIRX();break;
    case INS_ASL_ABX<<3|3:set_carry(cpu, cpu->DB);cpu->IRX = cpu->DB << 1;set_nz(cpu, cpu->IRX);break;
    case INS_ASL_ABX<<3|4:cpu->DB = cpu->IRX;WRITE();break;
    case INS_ASL_ABX<<3|5:_SYNC_ON();break;
    
    case INS_PLP_IMP<<3|0:POP();break;
    case INS_PLP_IMP<<3|1:RWSTACK();break;
    case INS_PLP_IMP<<3|2:cpu->P = cpu->DB;_SYNC_ON();break;

    case INS_JSP_AB<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_JSP_AB<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();FB();break;
    case INS_JSP_AB<<3|2:cpu->DB = (cpu->PC - 1) >> 8;RWSTACK();WRITE();PUSH();break;
    case INS_JSP_AB<<3|3:cpu->DB = (cpu->PC - 1) & 0xFF;RWSTACK();WRITE();PUSH();break;
    case INS_JSP_AB<<3|4:cpu->PC = cpu->IRX;_SYNC_ON();break;
    
    case INS_PHA_IMP<<3|0:cpu->DB = cpu->A;RWSTACK();WRITE();break;
    case INS_PHA_IMP<<3|1:PUSH();_SYNC_ON();break;
    
    case INS_JMP_AB<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_JMP_AB<<3|1:cpu->IRX |= (Word)cpu->DB << 8;cpu->PC = cpu->IRX;_SYNC_ON();break;

    case INS_RTS_IMP<<3|0:break;
    case INS_RTS_IMP<<3|1:POP();RWSTACK();break;
    case INS_RTS_IMP<<3|2:cpu->PC = cpu->DB;POP();RWSTACK();break;
    case INS_RTS_IMP<<3|3:cpu->PC |= (Word)cpu->DB << 8;RWSTACK();break;
    case INS_RTS_IMP<<3|4:PC();_SYNC_ON();break;
    
    case INS_PLA_IMP<<3|0:POP();break;
    case INS_PLA_IMP<<3|1:RWSTACK();break;
    case INS_PLA_IMP<<3|2:cpu->A = cpu->DB;set_nz(cpu, cpu->A);_SYNC_ON();break;

    case INS_JMP_IN<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_JMP_IN<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();cpu->AB = cpu->IRX;break;
    case INS_JMP_IN<<3|2:cpu->IRX = cpu->DB;cpu->AB++;break;
    case INS_JMP_IN<<3|3:cpu->IRX |= (Word)cpu->DB << 8;cpu->PC = cpu->IRX;_SYNC_ON();break;
    
    case INS_STY_ZP<<3|0:WRITE();cpu->AB = cpu->DB;cpu->DB = cpu->Y;PC();break;
    case INS_STY_ZP<<3|1:_SYNC_ON();break;

    case INS_STA_ZP<<3|0:WRITE();cpu->AB = cpu->DB;cpu->DB = cpu->A;PC();break;
    case INS_STA_ZP<<3|1:_SYNC_ON();break;
    
    case INS_STX_ZP<<3|0:WRITE();cpu->AB = cpu->DB;cpu->DB = cpu->X;PC();break;
    case INS_STX_ZP<<3|1:_SYNC_ON();break;
    
    case INS_DEY_IMP<<3|0:cpu->Y--;set_nz(cpu, cpu->Y);_SYNC_ON();break;
    
    case INS_TXA_IMP<<3|0:cpu->A=cpu->X;set_nz(cpu, cpu->A);_SYNC_ON();break;

    case INS_STY_AB<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_STY_AB<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();WRITE();cpu->DB = cpu->Y;cpu->AB = cpu->IRX;break;
    case INS_STY_AB<<3|2:_SYNC_ON();break;

    case INS_STA_AB<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_STA_AB<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();WRITE();cpu->DB = cpu->A;cpu->AB = cpu->IRX;break;
    case INS_STA_AB<<3|2:_SYNC_ON();break;
    
    case INS_STX_AB<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_STX_AB<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();WRITE();cpu->DB = cpu->X;cpu->AB = cpu->IRX;break;
    case INS_STX_AB<<3|2:_SYNC_ON();break;
    
    case INS_STY_ZPX<<3|0:cpu->AB = cpu->DB + cpu->X;PC();break;
    case INS_STY_ZPX<<3|1:WRITE();cpu->DB = cpu->Y;break;
    case INS_STY_ZPX<<3|2:_SYNC_ON();break;
    
    case INS_STA_ZPX<<3|0:cpu->AB = cpu->DB + cpu->X;PC();break;
    case INS_STA_ZPX<<3|1:WRITE();cpu->DB = cpu->A;break;
    case INS_STA_ZPX<<3|2:_SYNC_ON();break;
    
    case INS_STX_ZPY<<3|0:cpu->AB = cpu->DB + cpu->Y;PC();break;
    case INS_STX_ZPY<<3|1:WRITE();cpu->DB = cpu->X;break;
    case INS_STX_ZPY<<3|2:_SYNC_ON();break;
    
    case INS_TYA_IMP<<3|0:cpu->A=cpu->Y;set_nz(cpu, cpu->A);_SYNC_ON();break;
    
    case INS_STA_ABY<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_STA_ABY<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();cpu->IRX += cpu->Y;break;
    case INS_STA_ABY<<3|2:WRITE();cpu->AB = cpu->IRX;cpu->DB = cpu->A;break;
    case INS_STA_ABY<<3|3:;_SYNC_ON();break;

    case INS_TXS_IMP<<3|0:cpu->SP = cpu->X;_SYNC_ON();break;
    
    case INS_STA_ABX<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_STA_ABX<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();cpu->IRX += cpu->X;break;
    case INS_STA_ABX<<3|2:WRITE();cpu->AB = cpu->IRX;cpu->DB = cpu->A;break;
    case INS_STA_ABX<<3|3:;_SYNC_ON();break;
    
    case INS_LDY_IM<<3|0:cpu->Y = cpu->DB;PC();set_nz(cpu, cpu->Y);_SYNC_ON();break;
    
    case INS_LDX_IM<<3|0:cpu->X = cpu->DB;PC();set_nz(cpu, cpu->X);_SYNC_ON();break;

    case INS_LDY_ZP<<3|0:FBZ();PC();break;
    case INS_LDY_ZP<<3|1:cpu->Y = cpu->DB;set_nz(cpu, cpu->Y);_SYNC_ON();break;
    
    case INS_LDA_ZP<<3|0:FBZ();PC();break;
    case INS_LDA_ZP<<3|1:cpu->A = cpu->DB;set_nz(cpu, cpu->A);_SYNC_ON();break;
    
    case INS_LDX_ZP<<3|0:FBZ();PC();break;
    case INS_LDX_ZP<<3|1:cpu->X = cpu->DB;set_nz(cpu, cpu->X);_SYNC_ON();break;
    
    case INS_TAY_IMP<<3|0:cpu->Y=cpu->A;set_nz(cpu, cpu->Y);_SYNC_ON();break;

    case INS_LDA_IM<<3|0:cpu->A = cpu->DB;PC();set_nz(cpu, cpu->A);_SYNC_ON();break;

    case INS_TAX_IMP<<3|0:cpu->X=cpu->A;set_nz(cpu, cpu->X);_SYNC_ON();break;
    
    case INS_LDY_AB<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_LDY_AB<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();FBIRX();break;
    case INS_LDY_AB<<3|2:cpu->Y = cpu->DB;set_nz(cpu, cpu->Y);_SYNC_ON();break;
    
    case INS_LDA_AB<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_LDA_AB<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();FBIRX();break;
    case INS_LDA_AB<<3|2:cpu->A = cpu->DB;set_nz(cpu, cpu->A);_SYNC_ON();break;
    
    case INS_LDX_AB<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_LDX_AB<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();FBIRX();break;
    case INS_LDX_AB<<3|2:cpu->X = cpu->DB;set_nz(cpu, cpu->X);_SYNC_ON();break;
    
    case INS_LDY_ZPX<<3|0:FBZ();PC();break;
    case INS_LDY_ZPX<<3|1:FBX();break;
    case INS_LDY_ZPX<<3|2:cpu->Y = cpu->DB;set_nz(cpu, cpu->Y);_SYNC_ON();break;
    
    case INS_LDA_ZPX<<3|0:FBZ();PC();break;
    case INS_LDA_ZPX<<3|1:FBX();break;
    case INS_LDA_ZPX<<3|2:cpu->A = cpu->DB;set_nz(cpu, cpu->A);_SYNC_ON();break;

    case INS_LDX_ZPY<<3|0:FBZ();PC();break;
    case INS_LDX_ZPY<<3|1:FBY();break;
    case INS_LDX_ZPY<<3|2:cpu->X = cpu->DB;set_nz(cpu, cpu->X);_SYNC_ON();break;
    
    case INS_LDA_ABY<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_LDA_ABY<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();cpu->AB = cpu->IRX + cpu->Y;break;
    case INS_LDA_ABY<<3|2:if (PAGECROSS(cpu->AB, cpu->IRX))break;cpu->A = cpu->DB;_SYNC_ON();break;
    case INS_LDA_ABY<<3|3:cpu->A = cpu->DB;_SYNC_ON();break;
    
    case INS_TSX_IMP<<3|0:cpu->X = cpu->SP;set_nz(cpu, cpu->X);_SYNC_ON();break;

    case INS_LDY_ABX<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_LDY_ABX<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();cpu->AB = cpu->IRX + cpu->X;break;
    case INS_LDY_ABX<<3|2:if (PAGECROSS(cpu->AB, cpu->IRX))break;cpu->Y = cpu->DB;_SYNC_ON();break;
    case INS_LDY_ABX<<3|3:cpu->Y = cpu->DB;_SYNC_ON();break;
    
    case INS_LDA_ABX<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_LDA_ABX<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();cpu->AB = cpu->IRX + cpu->X;break;
    case INS_LDA_ABX<<3|2:if (PAGECROSS(cpu->AB, cpu->IRX))break;cpu->A = cpu->DB;_SYNC_ON();break;
    case INS_LDA_ABX<<3|3:cpu->A = cpu->DB;_SYNC_ON();break;
    
    case INS_LDX_ABY<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_LDX_ABY<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();cpu->AB = cpu->IRX + cpu->Y;break;
    case INS_LDX_ABY<<3|2:if (PAGECROSS(cpu->AB, cpu->IRX))break;cpu->X = cpu->DB;_SYNC_ON();break;
    case INS_LDX_ABY<<3|3:cpu->X = cpu->DB;_SYNC_ON();break;
    
    case INS_DEC_ZP<<3|0:FBZ();PC();break;
    case INS_DEC_ZP<<3|1:cpu->IRX = --cpu->DB;set_nz(cpu, cpu->IRX);break;
    case INS_DEC_ZP<<3|2:cpu->DB = cpu->IRX;WRITE();break;
    case INS_DEC_ZP<<3|3:_SYNC_ON();break;
    
    case INS_INY_IMP<<3|0:cpu->Y++;set_nz(cpu, cpu->Y);_SYNC_ON();break;

    case INS_DEX_IMP<<3|0:cpu->X--;set_nz(cpu, cpu->X);_SYNC_ON();break;

    case INS_DEC_AB<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_DEC_AB<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();FBIRX();break;
    case INS_DEC_AB<<3|2:cpu->IRX = --cpu->DB;set_nz(cpu, cpu->IRX);break;
    case INS_DEC_AB<<3|3:cpu->DB = cpu->IRX;WRITE();break;
    case INS_DEC_AB<<3|4:_SYNC_ON();break;

    case INS_DEC_ZPX<<3|0:FBZ();PC();break;
    case INS_DEC_ZPX<<3|1:FBX();break;
    case INS_DEC_ZPX<<3|2:cpu->IRX = --cpu->DB;set_nz(cpu, cpu->IRX);break;
    case INS_DEC_ZPX<<3|3:cpu->DB = cpu->IRX;WRITE();break;
    case INS_DEC_ZPX<<3|4:_SYNC_ON();break;

    case INS_DEC_ABX<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_DEC_ABX<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();FBIRX();break;
    case INS_DEC_ABX<<3|2:cpu->AB = cpu->IRX + cpu->X;break;
    case INS_DEC_ABX<<3|3:cpu->IRX = --cpu->DB;set_nz(cpu, cpu->IRX);break;
    case INS_DEC_ABX<<3|4:cpu->DB = cpu->IRX;WRITE();break;
    case INS_DEC_ABX<<3|5:_SYNC_ON();break;
    
    case INS_INC_ZP<<3|0:FBZ();PC();break;
    case INS_INC_ZP<<3|1:cpu->IRX = ++cpu->DB;set_nz(cpu, cpu->IRX);break;
    case INS_INC_ZP<<3|2:cpu->DB = cpu->IRX;WRITE();break;
    case INS_INC_ZP<<3|3:_SYNC_ON();break;
    
    case INS_INX_IMP<<3|0:cpu->X++;set_nz(cpu, cpu->X);_SYNC_ON();break;
    
    case INS_NOP_IMP<<3|0: _SYNC_ON(); break;

    case INS_INC_AB<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_INC_AB<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();FBIRX();break;
    case INS_INC_AB<<3|2:cpu->IRX = ++cpu->DB;set_nz(cpu, cpu->IRX);break;
    case INS_INC_AB<<3|3:cpu->DB = cpu->IRX;WRITE();break;
    case INS_INC_AB<<3|4:_SYNC_ON();break;
    
    case INS_INC_ZPX<<3|0:FBZ();PC();break;
    case INS_INC_ZPX<<3|1:FBX();break;
    case INS_INC_ZPX<<3|2:cpu->IRX = ++cpu->DB;set_nz(cpu, cpu->IRX);break;
    case INS_INC_ZPX<<3|3:cpu->DB = cpu->IRX;WRITE();break;
    case INS_INC_ZPX<<3|4:_SYNC_ON();break;

    case INS_INC_ABX<<3|0:cpu->IRX = cpu->DB;PC();FB();break;
    case INS_INC_ABX<<3|1:cpu->IRX |= (Word)cpu->DB << 8;PC();break;
    case INS_INC_ABX<<3|2:cpu->AB = cpu->IRX + cpu->X;break;
    case INS_INC_ABX<<3|3:cpu->IRX = ++cpu->DB;set_nz(cpu, cpu->IRX);break;
    case INS_INC_ABX<<3|4:cpu->DB = cpu->IRX;WRITE();break;
    case INS_INC_ABX<<3|5:_SYNC_ON();break;
    /* --------------------------- */

    /* CPU Flag Status changes should all be working */
    case INS_CLC_IMP<<3|0:cpu->P &= ~M6502_CF;_SYNC_ON();break;
    case INS_CLD_IMP<<3|0:cpu->P &= ~M6502_DF;_SYNC_ON();break;
    case INS_CLI_IMP<<3|0:cpu->P &= ~M6502_IF;_SYNC_ON();break;
    case INS_CLV_IMP<<3|0:cpu->P &= ~M6502_VF;_SYNC_ON();break;
    case INS_SEC_IMP<<3|0:cpu->P |= M6502_CF;_SYNC_ON();break;
    case INS_SED_IMP<<3|0:cpu->P |= M6502_DF;_SYNC_ON();break;
    case INS_SEI_IMP<<3|0:cpu->P |= M6502_IF;_SYNC_ON();break;
    /* --------------------------- */

    /* Interupt stuff needs to be tested */
    case INS_RTI_IMP<<3|0:POP();RWSTACK();break;
    case INS_RTI_IMP<<3|1:cpu->P = cpu->DB;POP();RWSTACK();break;
    case INS_RTI_IMP<<3|2:cpu->IRX = cpu->DB;POP();RWSTACK();break;
    case INS_RTI_IMP<<3|3:cpu->IRX |= (Word)cpu->DB << 8;break;
    case INS_RTI_IMP<<3|4:cpu->PC = cpu->IRX;_SYNC_ON();break;
    /* --------------- */
    /* To be implemented */
    case INS_STA_INX<<3|0: break;
    case INS_STA_INX<<3|1: break;
    case INS_STA_INX<<3|2: break;
    case INS_STA_INX<<3|3: break;
    case INS_STA_INX<<3|4:_SYNC_ON();break;

    case INS_STA_INY<<3|0: break;
    case INS_STA_INY<<3|1: break;
    case INS_STA_INY<<3|2: break;
    case INS_STA_INY<<3|3: break;
    case INS_STA_INY<<3|4:_SYNC_ON();break;
    /* ------------ */

    /* Hard to test */
    case INS_LDA_INX<<3|0:FBZ();PC();break;
    case INS_LDA_INX<<3|1:cpu->AB = cpu->DB + cpu->X;break;
    case INS_LDA_INX<<3|2:cpu->IRX = cpu->DB;cpu->AB++;break;
    case INS_LDA_INX<<3|3:cpu->IRX |= (Word)cpu->DB << 8;FBIRX();break;
    case INS_LDA_INX<<3|4: _SYNC_ON();cpu->A = cpu->DB;set_nz(cpu, cpu->A);_SYNC_ON();break;

    case INS_LDA_INY<<3|0:FBZ();PC();break;
    case INS_LDA_INY<<3|1:cpu->IRX = cpu->DB;cpu->AB++;break;
    case INS_LDA_INY<<3|2:cpu->IRX |= (Word)cpu->DB << 8;cpu->AB = cpu->IRX + cpu->Y;break;           
    case INS_LDA_INY<<3|3:if (PAGECROSS(cpu->AB, cpu->IRX))break;cpu->A = cpu->DB;_SYNC_ON();break;
    case INS_LDA_INY<<3|4:cpu->A = cpu->DB;_SYNC_ON();break;    
    /* ----------- */



    default:
        printf("Unknown instruction%02X\n", cpu->IR);
        //getchar();
        break;
    }    
}
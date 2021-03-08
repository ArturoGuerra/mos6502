#ifndef _CPU_H_
#define _CPU_H_

// 6502 Instructions 

/* Load/Store Operations */

// LDA - Load Accumulator
#define INS_LDA_IM 0xA9
#define INS_LDA_ZP 0xA5
#define INS_LDA_ZPX 0xB5
#define INS_LDA_AB 0xAD
#define INS_LDA_ABX 0xBD
#define INS_LDA_ABY 0xB9
#define INS_LDA_INX 0xA1
#define INS_LDA_INY 0xB1

// LDX - Load X Register
#define INS_LDX_IM 0xA2
#define INS_LDX_ZP 0xA6
#define INS_LDX_ZPY 0xB6
#define INS_LDX_AB 0xAE
#define INS_LDX_ABY 0xBE

// LDY - Load Y Register
#define INS_LDY_IN 0xA0
#define INS_LDY_ZP 0xA4
#define INS_LDY_ZPX 0xB4
#define INS_LDY_AB 0xAC
#define INS_LDY_ABX 0xBC

// STA - Store Accumulator
// Stores the contents of the accumulator into memory
#define INS_STA_ZP 0x85
#define INS_STA_ZPX 0x95
#define INS_STA_AB 0x8D
#define INS_STA_ABX 0x9D
#define INS_STA_ABY 0x99
#define INS_STA_INX 0x81
#define INS_STA_INY 0x91

// STX - Store X Register
// Stores the contents of the X register into memory
#define INS_STX_ZP 0x86
#define INS_STX_ZPY 0x96
#define INS_STX_AB 0x8E

// STY - Store Y Register
// Stores the contents of the Y register into memory
#define INS_STY_ZP 0x84
#define INS_STY_ZPX 0x94
#define INS_STY_AB 0x8C

/* Register Transfer */

// TAX - Transfer Accumulator to X
// Copies the current contents of the accumulator into the X Register
#define INS_TAX_IMP 0xAA

// TAY - Transfer Accumulator to Y
// Copies the current contents of the accumulator into the Y Register
#define INS_TAY_IMP 0xA8

// TXA - Transfer X to Accumulator
// Copies the current contents of the X register into the accumulator
#define INS_TXA_IMP 0x8A

// TYA - Transfer Y to Accumulator
// Copies the current contents of the Y register into the accumulator
#define INS_TYA_IMP 0x98

/* Stack Operations */ 

// TSX - Transfer Stack pointer to X
// Copies the current contents of the stack register into the X register
#define INS_TSX_IMP 0xBA

// TXS - Transfer X to Stack Pointer
// Copies the current contents of the X register into the Stack register
// No Processor Flags
#define INS_TXS_IMP 0x9A


// PHA - Push Accumulator
// Pushes a copy of the accumulator on to the stack
#define INS_PHA_IMP 0x48

// PHP - Push Processor Status
// Pushes a copy of the Processor Status(Flags) on to the stack
// Hopefully this is better then the other PHP
#define INS_PHP_IMP 0x08

// PLA - Pull Accumulator
// Pulls 8-bits from the stack into the accumulator
#define INS_PLA_IMP 0x68

// PLP - Pull Processor Status
// Pulls 8-bits from the stack into the Processors Status Flags
#define INS_PLP_IMP 0x28

/* Logic */

// AND - Logical AND
// A logical AND is performed bit by bit on the accumulator contents using the contents of a byte of memory
#define INS_AND_IM 0x29
#define INS_AND_ZP 0x25
#define INS_AND_ZPX 0x35
#define INS_AND_AB 0x2D
#define INS_AND_ABX 0x3D
#define INS_AND_ABY 0x39
#define INS_AND_INX 0x21
#define INS_AND_INY 0x31

// EOR - Exclusive OR
// An exclusive OR is performed bit by bit on the accumulator contents using the contents of a byte of memory
#define INS_EOR_IM 0x49
#define INS_EOR_ZP 0x45
#define INS_EOR_ZPX 0x55
#define INS_EOR_AB 0x4D
#define INS_EOR_ABX 0x5D
#define INS_EOR_ABY 0x59
#define INS_EOR_INX 0x41
#define INS_EOR_INY 0x51

// ORA - Logical Inclusive OR
// An inclusive OR is performed bit by bit on the accumulator contents using the contents of a byte of memory
#define INS_ORA_IM 0x09
#define INS_ORA_ZP 0x05
#define INS_ORA_ZPX 0x15
#define INS_ORA_AB 0x0D
#define INS_ORA_ABX 0x1D
#define INS_ORA_ABY 0x19
#define INS_ORA_INX 0x01
#define INS_ORA_INY 0x11

// BIT - Bit Test
// This instructions is used to test if one or more bits are set in a target memory location. The mask pattern in A is ANDed with the value in memory to set or clear the zero flag but the result is not kept. Bits 7 and 6 of the value from memory are copied into the N and V flags.
#define INS_BIT_ZP 0x24
#define INS_BIT_AB 0x2C

// Arithmetic
// ADC - Add with Carry
// This instruction adds the contents of a memory location to the accumulator together with the carry bit. If overflow occurs the carry bit is set this enables multiple byte addition to be performed.
#define INS_ADC_IM 0x69
#define INS_ADC_ZP 0x65
#define INS_ADC_ZPX 0x75
#define INS_ADC_AB 0x6D
#define INS_ADC_ABX 0x7D
#define INS_ADC_ABY 0x79
#define INS_ADC_INX 0x61
#define INS_ADC_INY 0x71

// SBC - Subtract with Carry
// This instruction subtracts the contents of a memory location to the accumulator together with the not of the carry bit. If overflow occurs the carry bit is clear this enables multiple byte subtraction to be performed.
#define INS_SBC_IM 0xE9
#define INS_SBC_ZP 0xE5 
#define INS_SBC_ZPX 0xF5
#define INS_SBC_AB 0xED
#define INS_SBC_ABX 0xFD
#define INS_SBC_ABY 0xF9
#define INS_SBC_INX 0xE1
#define INS_SBC_INY 0xF1

// CMP - Compare
// This instruction compares the contents of the accumulator with another memory held value and sets the zero and carry flags as appropriate.
#define INS_CMP_IM 0xC9
#define INS_CMP_ZP 0xC5
#define INS_CMP_ZPX 0xD5
#define INS_CMP_AB 0xCD
#define INS_CMP_ABX 0xDD
#define INS_CMP_ABY 0xD9
#define INS_CMP_INX 0xC1
#define INS_CMP_INY 0xD1

// CPX - Compare X Register
// This instruction compares the contents of the X register with another memory held value and sets the zero and carry flags as appropriate.
#define INS_CMX_IM 0xE0
#define INS_CMX_ZP 0xE4
#define INS_CMX_AB 0xEC

// CPY - Compare Y Register
// This instruction compares the contents of the Y register with another memory held value and sets the zero and carry flags as appropriate.
#define INS_CPY_IM 0xC0
#define INS_CPY_ZP 0xC4
#define INS_CPY_AB 0xCC

// INC - Increment Memory
// Adds one to the value held at a specified memory location setting the zero and negative flags as appropriate.
#define INS_INC_ZP 0xE6
#define INS_INC_ZPX 0xF6
#define INS_INC_AB 0xEE
#define INS_INC_ABX 0xFE
  
// INX - Increment X Register
// Adds one to the X register setting the zero and negative flags as appropriate.
#define INS_INX_IMP 0xE8

// INY - Increment Y Register
// Adds one to the Y register setting the zero and negative flags as appropriate.
#define INS_INY_IMP 0xC8

// DEC - Decrement Memory
// Subtracts one from the value held at a specified memory location setting the zero and negative flags as appropriate.
#define INS_DEC_ZP 0xC6
#define INS_DEC_ZPX 0xD6
#define INS_DEC_AB 0xCE
#define INS_DEC_ABX 0xDE

// DEX - Decrement X Register
// Subtracts one from the X register setting the zero and negative flags as appropriate.
#define INS_DEX_IMP 0xCA

// DEY - Decrement Y Register
// Subtracts one from the Y register setting the zero and negative flags as appropriate.
#define INS_DEY_IMP 0x88

// Shifts

// ASL - Arithmetic Shift Left
// This operation shifts all the bits of the accumulator or memory contents one bit left. Bit 0 is set to 0 and bit 7 is placed in the carry flag. The effect of this operation is to multiply the memory contents by 2 (ignoring 2's complement considerations) setting the carry if the result will not fit in 8 bits.
#define INS_ASL_ACC 0x0A
#define INS_ASL_ZP 0x06
#define INS_ASL_ZPX 0x16
#define INS_ASL_AB 0x0E
#define INS_ASL_ABX 0x1E

// LSR - Logical Shift Right
// Each of the bits in A or M is shift one place to the right. The bit that was in bit 0 is shifted into the carry flag. Bit 7 is set to zero.
#define INS_LSR_ACC 0x4A
#define INS_LSR_ZP 0x46
#define INS_LSR_ZPX 0x56
#define INS_LSR_AB 0x4E
#define INS_LSR_ABX 0x5E

// ROL - Rotate Left
// Move each of the bits in either A or M one place to the left. Bit 0 is filled with the current value of the carry flag whilst the old bit 7 becomes the new carry flag value.
#define INS_ROL_ACC 0x2A
#define INS_ROL_ZP 0x26
#define INS_ROL_ZPX 0x36
#define INS_ROL_AB 0x2E
#define INS_ROL_ABX 0x3E

// ROR - Rotate Right
// Move each of the bits in either A or M one place to the right. Bit 7 is filled with the current value of the carry flag whilst the old bit 0 becomes the new carry flag value.
#define INS_ROR_ACC 0x6A
#define INS_ROR_ZP 0x66
#define INS_ROR_ZPX 0x76
#define INS_ROR_AB 0x6E
#define INS_ROR_ABX 0x7E

// Jump and Calls

// JMP - Jump
// Sets the program counter to the address specified by the operand.
#define INS_JMP_AB 0x4C
#define INS_JMP_IN 0x6C

// JSR - Jump to Subroutine
// The JSR instruction pushes the address (minus one) of the return point on to the stack and then sets the program counter to the target memory address.
#define INS_JSP_AB 0x20

// RTS - Return from Subroutine
// The RTS instruction is used at the end of a subroutine to return to the calling routine. It pulls the program counter (minus one) from the stack.
#define INS_RTS_IMP 0x60

/* Branches */

// BCC - Branch if Carry Clear
// If the carry flag is clear then add the relative displacement to the program counter to cause a branch to a new location.
#define INS_BCC_REL 0x90

// BCS - Branch if Carry Set
// If the carry flag is set then add the relative displacement to the program counter to cause a branch to a new location.
#define INS_BCS_REL 0xB0

// BEQ - Branch if Equal
// If the zero flag is set then add the relative displacement to the program counter to cause a branch to a new location.
#define INS_BEQ_REL 0xF0

// BMI - Branch if Minus
// If the negative flag is set then add the relative displacement to the program counter to cause a branch to a new location.
#define INS_BMI_REL 0x30

// BNE - Branch if Not Equal
// If the zero flag is clear then add the relative displacement to the program counter to cause a branch to a new location.
#define INS_BNE_REL 0xD0

// BPL - Branch if Positive
// If the negative flag is clear then add the relative displacement to the program counter to cause a branch to a new location.
#define INS_BPL_REL 0x10

// BVC - Branch if Overflow Clear
// If the overflow flag is clear then add the relative displacement to the program counter to cause a branch to a new location.
#define INS_BVC_REL 0x50

// BVS - Branch if Overflow Set
// If the overflow flag is set then add the relative displacement to the program counter to cause a branch to a new location.
#define INS_BVS_REL 0x70

/* Status Flag Changes */

// CLC - Clear Carry Flag
// Set the carry flag to zero.
#define INS_CLC_IMP 0x18

// CLD - Clear Decimal Mode
// Sets the decimal mode flag to zero.
#define INS_CLD_IMP 0xD8

// CLI - Clear Interrupt Disable
// Clears the interrupt disable flag allowing normal interrupt requests to be serviced.
#define INS_CLI_IMP 0x58

// CLV - Clear Overflow Flag
// Clears the overflow flag.
#define INS_CLV_IMP 0xB8

// SEC - Set Carry Flag
// Set the carry flag to one.
#define INS_SEC_IMP 0x38

// SED - Set Decimal Flag
// Set the decimal mode flag to one.
#define INS_SED_IMP 0xF8

// SEI - Set Interrupt Disable
// Set the interrupt disable flag to one.
#define INS_SEI_IMP 0x78

/* System Functions */

// BRK - Force Interrupt
// The BRK instruction forces the generation of an interrupt request. The program counter and processor status are pushed on the stack then the IRQ interrupt vector at $FFFE/F is loaded into the PC and the break flag in the status set to one.
#define INS_BRK_IMP 0x00 // 7 cycles bois

// NOP - No Operation
// The NOP instruction causes no changes to the processor other than the normal incrementing of the program counter to the next instruction.
#define INS_NOP_IMP 0xEA // does the same thing as irl EA

// RTI - Return from Interrupt
// The RTI instruction is used at the end of an interrupt processing routine. It pulls the processor flags from the stack followed by the program counter.
#define INS_RTI_IMP 0x40

#define M6502_START_UP 7

typedef unsigned char Byte; // 8-Bit
typedef unsigned short Word; // 16-Bit
typedef unsigned int u32; // 32-Bit
typedef signed int s32; // 32-Bit

// pain generator
typedef struct m6502_s {
    Word PC; // Program Counter
    Byte SP; // Stack Pointer

    Byte A, X, Y; // Accumulator, Index Register X and Index Register Y;
    Byte Flags;

    // I/O
    Byte RDY : 1;
    Byte SYNC : 1;
    Byte RESET  : 1;
    // High = Read Low = Write
    Byte RW : 1; // Data bus RW bit
    Byte DB; // Data bus
    Word AB; // Address bus

    // Internal registers used to keep track of stuff
    Word IR; // Internal Instruction counter
    Word IRX; // Internal general purpose internal register
    Word IRY; // Internal general purpose internal register
    Byte INS; // Internal debug instruction pointer
} m6502_t;

void init_m6502(Word initVector, m6502_t *cpu);
void tick_m6502(m6502_t *cpu);

#endif
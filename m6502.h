#ifndef _M6502_H_
#define _M6502_H_

/*
Memory Map
   0x0000 - 0x00FF Zero Page
   0x0100 - 0x01FF Stack
   0X0200 - 0x1FFF General Memory

M6502 Pins:
   VSS : Ground Not needed
   RDY : Input : Ready
   (OUT) : Don't know what this is, will ignore for now :)
   IRQ : Input : Interrupt Request
   N.C. : Not Connected
   NMI : Input : Non-Maskable Interrupt
   SYNC : Output : Syncronize
   VCC : Power Not Needed
   A0 - A15 :  : Address Lines
   D7 - D0 :  : Data Lines
   R/W : Output : Read/Write
   N.C. : Not Connected
   N.C. : Not Connected
   (IN) : Don't know what this is, will ignore for now :)
   SO : Nobody knows what this pin does, will ignore it permanently :)
   (OUT) : Don't know what this is, will ignore for now :)
   RES : Input : Reset

   General I/O : 6
   Address Lines : Output : 16
   Data Lines : Both : 8
   Total I/O : 32   
*/

typedef unsigned char Byte;
typedef unsigned short Word;
typedef unsigned int PINS;

/* 
CPU Addressing Modes

*/
enum AddressingMode {
    Implied,
    Accumulator,
    Immediate,
    Absolute,
    ZeroPage,
    AbsoluteIndexedX,
    AbsoluteIndexedY,
    ZeroPageIndexedX,
    ZeroPageIndexedY,
    AbsoluteIndirect,
    IndexedIndirect,
    IndirectIndexed,
    Relative,
};

/*
CPU Status Flags

Used to alter the state of the CPU during a work load
*/

#define CARRY_FLAG     0x00
#define ZERO_FLAG      0x01
#define INTERRUPT_FLAG 0x02
#define DECIMAL_FLAG   0x03
#define BREAK_FLAG     0x04
#define OVERFLOW_FLAG  0x06
#define NEGATIVE_FLAG  0x07

struct StatusFlags {
    Byte C : 1; // Carry
    Byte Z : 1; // Zero
    Byte I : 1; // Interrupt
    Byte D : 1; // Decimal
    Byte B : 1; // Break
    Byte U : 1; // Unused
    Byte V : 1; // Overflow
    Byte N : 1; // Negative
};

/* CPU I/O Bus map takes a total of 32 bits */

#define RDY_PIN 0;
#define IRQ_PIN 1;
#define NMI_PIN 2;
#define SYNC_PIN 3;
#define AB_PIN 4;
#define DB_PIN 20;
#define RW_PIN 28;
#define RESET_PIN 29;

struct IOPins {
    Byte RDY : 1; // Ready
    Byte IRQ : 1; // Interrupt Request 
    Byte NMI : 1; // Non-Maskable Interrupt
    Byte SYNC : 1; // The cpu pull this HIGH during OPCODE fetching
    Word AB; // Address bus 16-bit
    Byte DB; // Bi-directional data bus 8-bit
    Byte RW : 1; // Data bus RW bit
    Byte RESET : 1; // Reset
};

/* Mos6502 CPU */

class M6502 {
public:
    M6502();

    // Ticks cpu for one clock cycle
    PINS tick(PINS pins);
    
    /* Internal CPU Registers */
    #ifdef DEBUG
    Word getPC() { return PC; };
    Word getSP() { return SP; };
    Byte getA()  { return A; };
    Byte getX()  { return X; };
    Byte getY()  { return Y; };
    AddressingMode getAddressingMode();
    #endif // DEBUG

private:
    Word PC; // Program Counter
    Byte SP; // Stack Pointer
    Byte A, X, Y; // Accumulator, Index Register X and Index Register Y;
    
    AddressingMode mode; // Current CPU Addressing mode set during instruction decode

    // CPU Status flags that can be address by either bit shifting or settings the flags directly    
    union {
        Byte F; // Status Flags
        StatusFlags flags;
    };

    // CPU I/O Bus
    union {
        IOPins IO;
        unsigned int io_pins;
    };
};

#endif // _M6502_H_
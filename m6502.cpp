#include "m6502.h"

M6502::M6502() {
    PC = 0xFFFC;
    SP = 0xFF;
    F = 0x00;
    A = X = Y = 0x00;
};

// Each tick is a single clock cycle
PINS M6502::tick(PINS pins) {
    return pins;
};
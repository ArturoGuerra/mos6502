from instructions import *

rom = bytearray([0xEA] * (1024 * 64))

rom[0xFFFC] = INS_LDA_AB
rom[0xFFFD] = 0xFF
rom[0xFFFE] = 0xAF
rom[0xAFFF] = 0x43

with open("rom.bin", "wb") as f:
    f.write(rom)
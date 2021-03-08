from instructions import INS_LDX_IM, INS_LDA_INX

rom = bytearray([0xEA] * (1024 * 64))

rom[0xFFFC] = 0x00
rom[0xFFFD] = 0x10

rom[0x1000] = INS_LDX_IM
rom[0x1001] = 0x69


#rom[0xFFFC] = INS_LDX_IM
#rom[0xFFFD] = 0x10
#rom[0xFFFE] = INS_LDA_INX
#rom[0xFFFF] = 0x43
#rom[0x00FA] = 0x10
#rom[0x00FB] = 0x10
#rom[0x1010] = 0x60


#    # 3 Cycles 2 Bytes
#    Memory[0xFFFC] = INS_LDA_ZP; # PC
#    Memory[0xFFFD] = 0xA1; # PC
#    Memory[0x00A1] = 0x22; # A r1egister should be 0x22
#
#
#    # 2 Cycles 2 Bytes
#    Memory[0xFFFE] = INS_LDX_IM; # PC
#    Memory[0xFFFF] = 0x10; # PC X register should be 0x10
#    
#    # 4 Cycles 2 Bytes
#    Memory[0x0000] = INS_LDA_ZPX; # PC
#    Memory[0x0001] = 0x67; # PC
#    Memory[0x0077] = 0x42; # X register should be 0x42
#
#    # 2 Cycles 2 Bytes
#    Memory[0x0002] = INS_LDA_IM; # PC
#    Memory[0x0003] = 0x69; # PC A 0x69


    # 4 Cycles 3 Bytes
#    Memory[0xFFFD] = 0xFF; # PC
#    Memory[0xFFFE] = 0xAF; # PC
#    Memory[0xAFFF] = 0x43;
#
#    Memory[0xFFFF] = INS_LDA_IM;
#    Memory[0x0000] = 0x30;

    # 2 Cycles 2 Bytes
#    Memory[0xFFFC] = INS_LDX_IM;
#    Memory[0xFFFD] = 0x10;
#    
#    # 4-5 Cycles 3 bytes
#    Memory[0xFFFE] = INS_LDA_ABX;
#    Memory[0xFFFF] = 0xFF;
#    Memory[0x0000] = 0x10;
#    Memory[0x110F] = 0x64;
#

with open("rom.bin", "wb") as f:
    f.write(rom)
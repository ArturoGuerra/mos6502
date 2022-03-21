
Rewrite to make the code's footprint smaller, and reuse more code, as the 6502 doesn't have many addressing modes
so I'll be adding an intruction decoder that will put the cpu in the correct addressing mode and execute the intructions from there,
intead of brute forcing everything instruction the cpu has by using a single switch that bit shifts as every intruction in the 6502 is separated by
exactly 7 bits which is what I currently use to select the correct instruction and intruction cycle.
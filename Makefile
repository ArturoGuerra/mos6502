.PHONY: all build cpu clean

BIN = m6502
OPTS = -Wall -std=c11 -O3 -g
GCC = gcc $(OPTS)

all: clean build

build: m6502
	$(GCC) -o $(BIN) m6502.o main.c

m6502:
	$(GCC) -c m6502.c -o m6502.o

clean:
	rm -rf m6502.o $(BIN)

vasm-test:
	vasm6502_oldstyle -Fbin -dotdir -o test.bin main.s

test:
	./m6502 test.bin

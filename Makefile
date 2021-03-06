.PHONY: all build cpu clean

BIN = m6502
OPTS = -Wall -std=c11 -O3 -g
GCC = gcc $(OPTS)

all: clean build

build: cpu
	$(GCC) -o $(BIN) cpu.o main.c

cpu:
	$(GCC) -c cpu.c -o cpu.o

clean:
	rm -rf cpu.o $(BIN)
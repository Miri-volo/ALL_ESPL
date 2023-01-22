
CFLAGS ?= -m32 -Wall -ansi -c -nostdlib -fno-stack-protector
main: main.o util.o start.o
	ld -m elf_i386 $^ -o $@

NASMFLAGS ?= -g -f elf -w+all
start.o: start.s
	nasm $(NASMFLAGS) -o $@ $<

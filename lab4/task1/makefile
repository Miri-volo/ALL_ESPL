task1: task1.o
	gcc -m32 -Wall -o task1 task1.o
task1.o: task1.asm
	nasm -g -f elf -w+all -o task1.o task1.asm
.PHONY: clean
clean:
	rm -f *.o task1
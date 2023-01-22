task0: task0.o
	gcc -m32 -Wall -o task0 task0.o
task0.o: task0.asm
	nasm -g -f elf -w+all -o task0.o task0.asm
.PHONY: clean
clean:
	rm -f *.o task0
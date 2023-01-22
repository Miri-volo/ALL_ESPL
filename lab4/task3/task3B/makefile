task3: task3.o
	gcc -m32 -Wall -g -o task3 task3.o
task3.o: task3.asm
	nasm -g -f elf -w+all -o task3.o task3.asm
.PHONY: clean
clean:
	rm -f *.o task3
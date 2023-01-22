loader: loader.o start.o
	ld -o $@ $^ -L/usr/lib32 -lc -T linking_script -dynamic-linker /lib32/ld-linux.so.2

loader.o: loader.c
	gcc -c -m32 -Wall -o $@ $^

start.o: start.s
	nasm -g -f elf -w+all -o $@ $^

.PHONY: clean
clean:
	rm -f *.o loader

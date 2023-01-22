objects = task0b.o

CFLAGS ?= -m32 -Wall -g -no-pie
main: $(objects)
	gcc $(CFLAGS) -o $@ $^

NASMFLAGS ?= -g -f elf -w+all
%.o: %.asm
	nasm $(NASMFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f *.o main

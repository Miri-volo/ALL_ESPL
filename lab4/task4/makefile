objects = task4.o

CFLAGS ?= -m32 -Wall -g -no-pie
task4: $(objects)
	gcc $(CFLAGS) -o $@ $^

NASMFLAGS ?= -g -f elf -w+all
%.o: %.asm
	nasm $(NASMFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f *.o task4

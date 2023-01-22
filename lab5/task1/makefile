objects = start.o

CFLAGS ?= -m32 -Wall -g -no-pie
main: $(objects)
	gcc $(CFLAGS) -o $@ $^

NASMFLAGS ?= -g -f elf -w+all
start.o: start.s
	nasm $(NASMFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f *.o main

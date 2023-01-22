task0: bubblesort.o hexaPrint.o
	gcc -m32 -g -Wall -o bubblesort bubblesort.o
	gcc -m32 -g -Wall -o hexaPrint hexaPrint.o
bubblesort.o: bubblesort.c
	gcc -g -m32 -Wall -c -o bubblesort.o bubblesort.c
hexaPrint.o: hexaPrint.c
	gcc -g -m32 -Wall -c -o hexaPrint.o hexaPrint.c
.PHONY: clean
clean:
	 rm -f *.o task0

task3: task3.o LineParser.o
	gcc -m32 -g -Wall -o task3 task3.o LineParser.o 
task3.o: task3.c
	gcc -g -Wall -m32 -c -o task3.o task3.c
LineParser.o: LineParser.c LineParser.h
	gcc -g -Wall -m32 -c -o LineParser.o LineParser.c 

.PHONY: clean
clean: 
	rm -f *.o task3

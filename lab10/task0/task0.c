#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int currentFD;
    void* mapStart;
} State;

int foreach_phdr(void *map_start, void (*func)(Elf32_Phdr *,int), int arg){
    int i;
    Elf32_Ehdr *header = (Elf32_Ehdr*)map_start;
    Elf32_Phdr *pointer = (Elf32_Phdr*)(map_start + header->e_phoff);
    for (i=0; i<header->e_phnum; i++) {
        func((Elf32_Phdr*)((void*)pointer + i*header->e_phentsize), i); 
    }
    return 0;
}

//same as readelf -l 
void checkForTask0(Elf32_Phdr* header, int index) {
    printf("Program header number %d at address %x\n", index, header -> p_vaddr);
}

int main(int argc, char **argv)
{
    //from lab 9
    State *s = malloc(sizeof(State));
    s->currentFD = open(argv[1], 0);
    if (s->currentFD <= 0) {
        fprintf(stderr, "Error: Failed opening ELF file.\n");
        return 0;
    }
    struct stat fStat;
    if (fstat(s->currentFD, &fStat) != 0) {
        fprintf(stderr, "Error: Failed during fstat.\n");
        return 0;
    }
    s->mapStart = mmap(NULL, fStat.st_size, PROT_READ, MAP_SHARED, s->currentFD, 0);
    if (s->mapStart == MAP_FAILED) {
        fprintf(stderr, "Error: Failed during mmap.\n");
        return 0;
    }
    foreach_phdr(s->mapStart,checkForTask0,0);
    return 0;
}
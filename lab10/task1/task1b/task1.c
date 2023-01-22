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

void readlf_lPrinter(Elf32_Phdr* header, int index) {
    char * protectionsFlags = "";
    //https://man7.org/linux/man-pages/man5/elf.5.html
    if(header->p_type == PT_NULL){printf("NULL     ");}
    else if(header->p_type == PT_LOAD){printf("LOAD     ");}
    else if(header->p_type == PT_DYNAMIC){printf("DYNAMIC  ");}
    else if(header->p_type == PT_INTERP){printf("INTERP   ");}
    else if(header->p_type == PT_NOTE){printf("NOTE     ");}
    else if(header->p_type == PT_SHLIB){printf("SHLIB    ");}
    else if(header->p_type == PT_PHDR){printf("PHDR     ");}
    else if(header->p_type == PT_LOPROC){printf("LOPROC   ");}
    else if(header->p_type == PT_HIPROC){printf("HIPROC   ");}
    else if(header->p_type == PT_GNU_STACK){printf("GNU_STACK");}
    else {printf("         ");}

    printf(" 0x%06x 0x%08x 0x%08x 0x%05x 0x%05x ",
        header->p_offset,
        header->p_vaddr,
        header->p_paddr,
        header->p_filesz,
        header->p_memsz);
    
    //https://docs.oracle.com/cd/E19683-01/816-1386/chapter6-83432/index.html
    if (header->p_flags == PF_X) {
        printf("E   ");
        protectionsFlags = "PROT_EXEC";
    } 
    else if (header->p_flags == PF_W) {
        printf("W   ");
        protectionsFlags = "PROT_WRITE";
    } 
    else if (header->p_flags == PF_W + PF_X) {
        printf("WE  ");
        protectionsFlags = "PROT_WRITE|PROT_EXEC";
    }
    else if ( header->p_flags == PF_R) {
        printf("R   ");
        protectionsFlags = "PROT_READ";
    } 
    else if (header->p_flags == PF_R + PF_X) {
        printf("RE  ");
        protectionsFlags = "PROT_READ|PROT_EXEC";
    } 
    else if (header->p_flags ==PF_R + PF_W) {
        printf("RW  ");
        protectionsFlags = "PROT_READ|PROT_WRITE";
    }
    else if (header->p_flags ==PF_R + PF_W + PF_X) {
        printf("RWE ");
        protectionsFlags = "PROT_READ|PROT_WRITE|PROT_EXEC";
    }
    else{
        printf("    ");
    }

    printf("0x%x ", header->p_align);
    //task1b
    //https://man7.org/linux/man-pages/man2/mmap.2.html for protectionsFlags
    //MAP_PRIVATE|MAP_FIXED for PT_LOAD from reading material
    if(header->p_type == PT_LOAD){
        printf("MAP_PRIVATE|MAP_FIXED %s\n", protectionsFlags);
    }
    else{
        printf("\n");
    }
}

void readlf_l(Elf32_Phdr* header) {
    printf("Type      Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align  mappingFlags          protectionFlags\n");
    foreach_phdr(header,readlf_lPrinter,0);
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
    readlf_l(s->mapStart);
    return 0;
}
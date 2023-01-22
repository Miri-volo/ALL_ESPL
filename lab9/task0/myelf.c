#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 
#include <sys/mman.h>
#include <elf.h>

typedef struct {
    char debugMode;
    int currentFD;
    void* mapStart;
} State;

typedef struct {
    char *name;
    void (*fun)(State*);
} FunDesc;

void displayMenu(FunDesc *menu){
    int i=0;
    fprintf(stderr,"Choose actoin:\n");
    while(menu[i].name != NULL){
        fprintf(stderr,"%i-%s\n",i, menu[i].name);
        i++;
    }
}

void toggleDebugMode(State* s){
    if (s->debugMode == 1)
    {
        s->debugMode = 0;
        fprintf(stderr,"Debug flag now off\n");
    }
    else if (s->debugMode == 0)
    {
        s->debugMode = 1;
        fprintf(stderr,"Debug flag now on\n");
    }
}

void examineElfFile(State* s) {
    char fileNameLine[512];
    char fileName[512];
    fprintf(stderr,"Enter ELF file to examine:\n");
    fgets(fileNameLine, sizeof(fileNameLine), stdin);
    sscanf(fileNameLine, "%s", fileName);
    s->currentFD = open(fileName, 0);
    if (s->currentFD <= 0) {
        fprintf(stderr, "Error: Failed opening ELF file.\n");
        return;
    }
    struct stat fStat;
    if (fstat(s->currentFD, &fStat) != 0) {
        fprintf(stderr, "Error: Failed during fstat.\n");
        return;
    }
    s->mapStart = mmap(NULL, fStat.st_size, PROT_READ, MAP_SHARED, s->currentFD, 0);
    if (s->mapStart == MAP_FAILED) {
        fprintf(stderr, "Error: Failed during mmap.\n");
        return;
    }
    Elf32_Ehdr* header = s->mapStart;
    printf("Magic numbers: %X %X %X.\n", header->e_ident[1], header->e_ident[2], header->e_ident[3]);
    if (header->e_ident[1] != 0x45 || header->e_ident[2] != 0x4c || header->e_ident[3] != 0x46) {
        fprintf(stderr, "Error: Bad ELF header magic numbers.\n");
        return;
    }
    printf("Encoding scheme: %d\n", header->e_ident[5]);
    printf("Entrypoint: %X\n", header->e_entry);
    printf("Section offset: %d\n", header->e_shoff);
    printf("Section count: %d\n", header->e_shnum);
    printf("Section entry size: %d\n", header->e_shentsize);
    printf("Program header offset: %d\n", header->e_phoff);
    printf("Program header count: %d\n", header->e_phnum);
    printf("Program header entry size: %d\n", header->e_phentsize);
}

void printSectionNames(State* s) {
    fprintf(stderr, "Not implemented yet.\n");
}

void printSymbols(State* s) {
    fprintf(stderr, "Not implemented yet.\n");
}

void relocationTables(State* s) {
    fprintf(stderr, "Not implemented yet.\n");
}

void quit(State* s){
    if (s->debugMode == 1)
    {
        fprintf(stderr,"quitting\n");
    }
    exit(0);
}


int main(int argc, char **argv)
{
    State *s = malloc(sizeof(State));
    FunDesc menu[] = {
        {"Toggle Debug Mode", toggleDebugMode},
        {"Examine ELF File", examineElfFile},
        {"Print Section Names", printSectionNames},
        {"Print Symbols", printSymbols},
        {"Relocation Tables", relocationTables},
        {"Quit", quit},
        { NULL, NULL }};
    char input;
    char tempInput[10];
    int sizeOfmenu = sizeof(menu) / sizeof(FunDesc) - 1;
    while (1) {
        displayMenu(menu);
        fgets(tempInput, sizeof(tempInput), stdin);
        sscanf(tempInput, "%c", &input);
        if((int)input-48>=0 && (int)input-48 <sizeOfmenu){
            printf("Option : %i\n", (int)input-48);
            printf("Within bounds\n");
            if(s!=NULL){
                menu[(int)input-48].fun(s);
            }
            printf("DONE.\n");
        }
        else{
            fprintf(stderr,"Not within bounds\n");
            exit(0);
        }
    }
    return 0;
}


#include <stdio.h>
#include <stdlib.h>

void PrintHex(unsigned char* buffer,int length, FILE* output)
{
    for(int i=0;i<length;i++)
        fprintf(output, "%hhX ",buffer[i]);
    fprintf(output, "\n");
}

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

virus* readVirus(FILE* file){
    int error;
    virus* v = (virus*)malloc(sizeof(virus));
    error = fread(v, sizeof(char), 18, file);
    if(error == 0)
        exit(1);
    (*v).sig = (unsigned char*) malloc(sizeof(char)*(*v).SigSize);
    error = fread((*v).sig, sizeof(char), (*v).SigSize, file);
    if(error == 0)
        exit(1);
    return v;
}

void printVirus(virus* virus, FILE* output){
    fprintf(output, "Virus name: %s\nVirus size: %d\nsignature:\n", (*virus).virusName, (*virus).SigSize);
    PrintHex((*virus).sig, (*virus).SigSize, output);
}

int main(int argc, char **argv)
{
    FILE* input;
    FILE* output;
    char fname[50];
    char fname2[50];
    printf("Please enter input file name: \n");
    scanf("%s", fname);
    printf("Please enter output file name: \n");
    scanf("%s", fname2);
    input = fopen(fname,"r");
    output = fopen(fname2, "w+");
    if(input == NULL) {
        perror("File cannot be opened.\n");
        return 0;
    }
    while(1){
        virus* v = readVirus(input);
        printVirus(v, output);
        free((*v).sig);
        free(v);
    }
    fclose(input);
    fclose(output);
    return 0;
}
// NAME
//     hexaPrint - prints the hexdecimal value of the input bytes from a given file
// SYNOPSIS
//     hexaPrint FILE
// DESCRIPTION
//     hexaPrint receives, as a command-line argument,
//  the name of a "binary" file, and prints the hexadecimal value of each byte to the standard output,
//  separated by spaces.

#include <stdio.h>
#include <stdlib.h>

void PrintHex(char* buffer,int length)
{
    for(int i=0;i<length;i++)
        printf("%hhX ",buffer[i]);//https://stackoverflow.com/questions/8060170/printing-hexadecimal-characters-in-c
    printf("\n");
}

int main(int argc, char **argv)
{
    FILE* file = fopen(argv[1],"rb");;
    char* buffer;
    int length;
    int sizeForBuffer;
    if(file == NULL) {
        perror("File cannot be opened.\n");
        return 0;
    }
    fseek(file , 0 , SEEK_END);
    sizeForBuffer = ftell(file);
    buffer = (char*) malloc (sizeof(char)*sizeForBuffer);
    fseek(file,0,SEEK_SET);
    length = fread(buffer,1,sizeForBuffer,file);
    PrintHex(buffer,length);
    free(buffer);
    fclose(file);
    return 0;
}

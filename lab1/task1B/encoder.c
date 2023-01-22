// NAME
// encoder - encodes the input text as hexadecimal values.
// SYNOPSIS
// encoder [OPTION]...
// DESCRIPTION
// encoder receives text characters from standard input and prints the corresponding uppercase charachters to the standard output.
// The debug mode is activated via command-line argument (-D).
// If the debug-mode is activated, print the command-line arguments to stderr and each character you recieve from the input (hexadecimal value) before and after the conversion.
// regardless of the debug-mode, the encoder will convert characters into uppercase.
// EXAMPLES
// #> encoder -D
// -D
// Hi, my name is Noah
// 48 48
// 69 49
// 2C 2C
// 20 20
// 60 40
// 79 59
// 20 20
// 6E 4E
// 61 41
// 6D 4D
// 65 45
// 20 20
// 69 49
// 73 53
// 20 20
// 4E 4E
// 6F 4F
// 61 41
// 68 48

// HI, MY NAME IS NOAH
// ^D
// #>
#include <string.h>
#include <stdio.h>
int main(int argc, char **argv)
{
    int debug = -1;
    char words[256]; //like the example of number.c
    char word;
    int i = 0;
    if(argc==2){
        if((debug=strcmp(argv[1],"-D"))==0)
            printf("-D\n");
        else{
            printf("Wrong command, try 'encoder -D' or 'encoder'\n");
            return 0;
        }
    }
    if(argc>2){
        printf("Wrong command, try 'encoder -D' or 'encoder'\n");
        return 0;
    }
    while((word=fgetc(stdin))!=EOF){
        if(word=='\n'){
            for(int a=0; a < i; a++)
                printf("%c",words[a]);
            i=0;
            printf("\n");
        }
        else{
            words[i]=word;
            if((int)word > 96 && (int)word < 123){
                    char wordBefore = word;
                    word = (char)((int)word - 32);
                    words[i] = word;
                    //%x - hexadecimal
                    //https://en.m.wikibooks.org/wiki/C_Programming/Simple_input_and_output
                    if(debug==0)
                        printf("%X %X\n", wordBefore, word);
                }
            else{
                if(debug==0)
                    printf("%X %X\n", word, word);
            }
            i++;
        }
    }
    return 0;
}

// NAME
// encoder - encodes the input text as uppercase letters.
// SYNOPSIS
// encoder
// DESCRIPTION
// encoder reads the characters from standard input and prints the corresponding uppercase characters to the standard output. Non lowercase characters remain unchanged.
// EXAMPLES
// #> encoder
// Hi, my name is Noah
// #> HI, MY NAME IS NOAH
// #> ^D
// #>
#include <stdio.h>

int main(void)
{
    char word;
    while((word=fgetc(stdin))!=EOF) {
        if(word == '\n')
            printf("\n");
        else {
            if((int)word > 96 && (int)word < 123)
                word = (char)((int)word - 32);
            printf("%c",word);
        }
    }
    return 0;
}
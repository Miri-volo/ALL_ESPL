// # NAME
// # my_echo - echoes text.
// # SYNOPSIS
// # my_echo
// # DESCRIPTION
// # my_echo prints out the text given in the command line by the user.
// # EXAMPLES
// # #> my_echo aa b c
// # #> aa b c
#include <stdio.h>

int main(int argc, char **argv)
{
    for(int i=1; i<argc ; i++){
        if(i!= argc-1)
            printf("%s ",argv[i]);
        else
           printf("%s\n",argv[i]); 
    } 
    return 0;
}
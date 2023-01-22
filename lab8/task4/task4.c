#include <stdio.h>
#include <stdlib.h>
int digit_cnt(char *digits)
{
    int count = 0;
    int i = 0;
    while (digits[i]!='\0')
    {
        if (digits[i] >= 48 && digits[i] <= 57)
        {
            count++;
        }
        i++;
    }
    return count;
}

int main(int argc, char **argv)
{
    if(argc > 1){
        fprintf(stdout,"The number of digits in the string is: %d\n", digit_cnt(argv[1]));
    }
}
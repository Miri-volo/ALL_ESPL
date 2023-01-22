#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int pipefd[2];
    char text[6];
    int pid;
    if (pipe(pipefd) == -1)
    {
        perror("Unable to create pipe\n");
        exit(1);
    }
    pid = fork();
    if (pid == 0)
    {
        write(pipefd[1], "hello\n", 6); /*write end of the pipe*/
        _exit(0);
    }
    else
    {
        read(pipefd[0], &text, 6);
        write(STDOUT_FILENO, &text, 6);
        exit(0);
    }
}
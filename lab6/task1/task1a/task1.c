#include "LineParser.h"
#include "linux/limits.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int debug = -1;

void execute(cmdLine *pCmdLine) {
    int pid = fork();
    if (pid == 0)
    {
        if(debug == 0){
            printf("Returned from fork(): %d\n", pid);
            printf("Child PID: %d Parent PID: %d\n", getpid(),getppid());
        }
        if(execvp((*pCmdLine).arguments[0], (*pCmdLine).arguments) < 0){
            perror("error execvp fails");
            freeCmdLines(pCmdLine);
            _Exit(1);
        }
    }
}

int main(int argc, char **argv)
{
    cmdLine* line;
    char buf [PATH_MAX];
    char buf2[2048];
    if(argc > 1){
        debug = strncmp(argv[1], "-d", 2);
    }
    getcwd(buf, PATH_MAX);
    printf("The current working directory: %s\n", buf);
    while(1){
        fgets(buf2, sizeof(buf2), stdin);
        if(debug == 0){
            printf("Executing command: %s", buf2);
        }
        line = parseCmdLines(buf2);
        if (strncmp(buf2, "quit", 4) == 0)
        {
            freeCmdLines(line);
            exit(0);
        }
        if(line != NULL){
            execute(line);
        }
    }
    return 0;
}
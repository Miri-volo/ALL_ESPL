#include "LineParser.h"
#include "linux/limits.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void execute(cmdLine *pCmdLine) {
    /*execv((*pCmdLine).arguments[0], (*pCmdLine).arguments);*/
    execvp((*pCmdLine).arguments[0], (*pCmdLine).arguments);
    perror("error execv fails");
    freeCmdLines(pCmdLine);
    exit(1);
}

int main(int argc, char **argv)
{
    cmdLine* line;
    char buf [PATH_MAX];
    char buf2[2048];
    getcwd(buf, PATH_MAX);
    puts(buf);
    while(1){
        fgets(buf2, sizeof(buf2), stdin);
        line = parseCmdLines(buf2);
        if (strncmp(buf2, "quit", 4) == 0)
        {
            freeCmdLines(line);
            exit(0);
        }
        if(line != NULL)
            execute(line);
    }
    return 1;
}
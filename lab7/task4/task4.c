#include "LineParser.h"
#include "linux/limits.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define HISTLEN 5
char* history[HISTLEN];
int newestHistory = 0;
int oldestHistory = 0;
int historyCount = 0;

void pushHistory(char* command) {
    newestHistory = (newestHistory + 1) % HISTLEN;
    historyCount++;
    if (newestHistory == oldestHistory) {
        free(history[oldestHistory]);
        oldestHistory = (oldestHistory + 1) % HISTLEN;
        historyCount--;
    }
    history[newestHistory] = malloc(2048 * sizeof(char));
    memcpy(history[newestHistory], command, 2048 * sizeof(char));
}

char* getHistory(int index) {
    int i = newestHistory - (index - 1);
    if (i < 0) {
        i += HISTLEN;
    } else {
        i = i % HISTLEN;
    }
    return history[i];
}

int checkHistoryIndex(int index) {
    if (0 < index && index <= historyCount) {
        return 0;
    }
    return 1;
}

void printHistory() {
    int current = newestHistory;
    int currentIndex = 1;
    while (current != oldestHistory) {
        printf("%d: %s", currentIndex, history[current]);
        current--;
        if (current < 0) {
            current = HISTLEN - 1;
        }
        currentIndex++;
    }
    if (history[oldestHistory] != NULL) {
        printf("%d: %s", currentIndex, history[oldestHistory]);
    }
}

void freeHistory() {
    for (int i = 0; i < HISTLEN; i++) {
        if (history[i] != NULL) {
            free(history[i]);
        }
    }
    newestHistory = 0;
    oldestHistory = 0;
    historyCount = 0;
}

int debug = -1;

void execute(cmdLine *pCmdLine, int pipedFD) {
    int pipeFD[2];
    if (pCmdLine->next != NULL) {
        if (pipe(pipeFD) != 0) {
            perror("Unable to create pipe\n");
            return;
        }
    }
    int pid = fork();
    if (pid == 0)
    {
        if(debug == 0){
            printf("Returned from fork(): %d\n", pid);
            printf("Child PID: %d Parent PID: %d\n", getpid(),getppid());
        }
        if((*pCmdLine).inputRedirect != NULL) 
        {
            if(debug == 0){
                printf("InputRedirect: %s\n", (*pCmdLine).inputRedirect);
            }
            fclose(stdin);
            fopen((*pCmdLine).inputRedirect,"r");
        }
        if (pipedFD != -1) {
            fclose(stdin);
            dup(pipedFD);
            close(pipedFD);
        }
        if((*pCmdLine).outputRedirect != NULL) 
        {
            if(debug == 0){
                printf("outputRedirect: %s\n", (*pCmdLine).outputRedirect);
            }
            fclose(stdout);
            fopen((*pCmdLine).outputRedirect,"w");
        }
        if (pCmdLine->next != NULL) {
            fclose(stdout);
            dup(pipeFD[1]);
            close(pipeFD[1]);
        }
        if(execvp((*pCmdLine).arguments[0], (*pCmdLine).arguments) < 0){
            perror("error execvp fails");
            freeCmdLines(pCmdLine);
            _Exit(1);
        }
    }
    else
    {
        if((*pCmdLine).blocking != 0)
        {
            int * status = 0;
            waitpid(pid, status, 0);
        }
        if (pCmdLine->next != NULL) {
            close(pipeFD[1]);
        }
        if (pCmdLine->next != NULL) {
            execute(pCmdLine->next, pipeFD[0]);
        }
        if (pipedFD != -1) {
            close(pipedFD);
        }
    }
}

int main(int argc, char **argv)
{
    cmdLine* line;
    char buf [PATH_MAX];
    char buf2[2048];
    int status;
    if(argc > 1){
        debug = strncmp(argv[1], "-d", 2);
    }
    while(1){
        getcwd(buf, PATH_MAX);
        printf("The current working directory: %s\n", buf);
        fgets(buf2, sizeof(buf2), stdin);
        if(debug == 0){
            printf("Executing command: %s", buf2);
        }
        if (strncmp(buf2, "!!", 2) == 0) {
            if (checkHistoryIndex(1) != 0) {
                fprintf(stderr, "Error: !! must be used with non-empty history.\n");
                continue;
            }
            char* command = getHistory(1);
            printf("%s", command);
            memcpy(buf2, command, 2048 * sizeof(char));
        } else if (strncmp(buf2, "!", 1) == 0) {
            int index = atoi(buf2 + 1);
            if (index == 0) {
                fprintf(stderr, "Error: ! must be followed by a number.\n");
                continue;
            }
            if (checkHistoryIndex(index) != 0) {
                fprintf(stderr, "Error: ! index must be within history bounds.\n");
                continue;
            }
            char* command = getHistory(index);
            printf("%s", command);
            memcpy(buf2, command, 2048 * sizeof(char));
        }
        pushHistory(buf2);
        line = parseCmdLines(buf2);
        if (line == NULL) {
            freeCmdLines(line);
            continue;
        }
        if (strncmp((*line).arguments[0], "quit", 4) == 0)
        {
            freeCmdLines(line);
            freeHistory();
            exit(0);
        }
        else if (strncmp((*line).arguments[0], "cd", 2) == 0)
        {
            status = chdir((*line).arguments[1]);
            if (status == -1)
            {
                fprintf(stderr, "Command cd failed with status %d \n", status);
            }
        } else if (strncmp(line->arguments[0], "history", 7) == 0) {
            printHistory();
        }
        else if(line != NULL){
            execute(line, -1);
        }
        freeCmdLines(line);
    }
    return 0;
}

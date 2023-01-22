#include "LineParser.h"
#include "linux/limits.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0

char* statusToString(int status) {
    if (status == TERMINATED) {
        return "Terminated";
    }
    if (status == RUNNING) {
        return "Running";
    }
    if (status == SUSPENDED) {
        return "Suspended";
    }
    return "Invalid";
}

typedef struct process{
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;

void freeProcessList(process* proc) {
    if (proc->next) {
        freeProcessList(proc->next);
    }
    freeCmdLines(proc->cmd);
    free(proc);
}

void updateProcesses(process *proc) {
    int status = 0;
    int result = waitpid(proc->pid, &status, WNOHANG | WUNTRACED | WCONTINUED);
    if (0 < result) {
        if (WIFSTOPPED(status)) {
            proc->status = SUSPENDED;
        } else if (WIFCONTINUED(status)) {
            proc->status = RUNNING;
        } else if (WIFEXITED(status) || WIFSIGNALED(status)) {
            proc->status = TERMINATED;
        }
    }
    if (proc->next) {
        updateProcesses(proc->next);
    }
}

void updateProcessList(process **process_list) {
    updateProcesses(*process_list);
}

void updateProcessStatus(process* process_list, int pid, int status) {}

process* deleteTerminatedProcesses(process* proc) {
    if (proc == NULL) {
        return proc;
    }
    if (proc->status == TERMINATED) {
        return deleteTerminatedProcesses(proc->next);
    }
    proc->next = deleteTerminatedProcesses(proc->next);
    return proc;
}

void addProcess(process** process_list, cmdLine* cmd, pid_t pid, int status) {
    if ((*process_list) == NULL) {
        (*process_list) = malloc(sizeof(process));
        (*process_list)->next = 0;
        (*process_list)->cmd = cmd;
        (*process_list)->pid = pid;
        (*process_list)->status = status;
        return;
    }
    process* last = (*process_list);
    while(last->next) {
        last = last->next;
    }
    last->next = malloc(sizeof(process));
    last->next->next = 0;
    last->next->cmd = cmd;
    last->next->pid = pid;
    last->next->status = status;
}

void printCommand(cmdLine cmd) {
    for (int i = 0; i < cmd.argCount; i++) {
        printf("%s ", cmd.arguments[i]);
    }
}

int debug = -1;

void execute(cmdLine *pCmdLine, process** process_list) {
    int pid = fork();
    if (pid == 0)
    {
        if(debug == 0){
            printf("Returned from fork(): %d\n", pid);
            printf("Child PID: %d Parent PID: %d\n", getpid(),getppid());
        }
        if(execvp((*pCmdLine).arguments[0], (*pCmdLine).arguments) < 0){
            perror("error execvp fails");
            _Exit(1);
        }
    }
    else
    {
        if((*pCmdLine).blocking != 0)
        {
            addProcess(process_list, pCmdLine, pid, TERMINATED);
            int * status = 0;
            waitpid(pid, status, 0);
        } else {
            addProcess(process_list, pCmdLine, pid, RUNNING);
        }
    }
}

void printProcessSublist(process* proc) {
    if (proc == NULL) {
        return;
    }
    printf("%d %s ", proc->pid, statusToString(proc->status));
    printCommand(*(proc->cmd));
    printf("\n");
    printProcessSublist(proc->next);
}

void printProcessList(process** process_list) {
    printf("PID STATUS COMMAND\n");
    if (*process_list == NULL) {
        return;
    }
    updateProcessList(process_list);
    printProcessSublist(*process_list);
    *process_list = deleteTerminatedProcesses(*process_list);
}

int main(int argc, char **argv)
{
    cmdLine* line;
    char buf [PATH_MAX];
    char buf2[2048];
    int status;
    process* processes = NULL;

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
        line = parseCmdLines(buf2);
        if (strncmp((*line).arguments[0], "quit", 4) == 0)
        {
            freeCmdLines(line);
            freeProcessList(processes);
            exit(0);
        }
        else if (strncmp((*line).arguments[0], "cd", 2) == 0)
        {
            status = chdir((*line).arguments[1]);
            if (status == -1)
            {
                fprintf(stderr, "Command cd failed with status %d \n", status);
            }
            freeCmdLines(line);
        }
        else if (strncmp((*line).arguments[0], "procs", 5) == 0) {
            printProcessList(&processes);
            freeCmdLines(line);
        }
        else if (strncmp((*line).arguments[0], "suspend", 7) == 0) {
            int pid = atoi(line->arguments[1]);
            kill(pid, SIGTSTP);
            freeCmdLines(line);
        }
        else if (strncmp((*line).arguments[0], "kill", 4) == 0) {
            int pid = atoi(line->arguments[1]);
            kill(pid, SIGINT);
            freeCmdLines(line);
        }
        else if (strncmp((*line).arguments[0], "wake", 4) == 0) {
            int pid = atoi(line->arguments[1]);
            kill(pid, SIGCONT);
            freeCmdLines(line);
        }
        else if(line != NULL){
            execute(line, &processes);
        }
    }
    return 0;
}

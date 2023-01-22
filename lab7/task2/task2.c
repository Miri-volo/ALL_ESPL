#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/*pipefd[0] refers to the read end of the pipe.  
pipefd[1] refers to the write end of the pipe.*/
int main(int argc, char *argv[])
{
    char * argumentsLs []= {"ls", "-l", 0};
    char * argumentsTail []= {"tail", "-n", "2", 0};
    int pipefd[2];
    int pid;
    int pid2;
    int debug;
    if(argc > 1){
        debug = strncmp(argv[1], "-d", 2);
    }
    if (pipe(pipefd) == -1) /*1*/
    {
        perror("Unable to create pipe\n");
        exit(1);
    }
    if(debug == 0){
        fprintf(stderr,"(parent_process>forking…)\n");
    }
    pid = fork();  /*2*/
    if(debug == 0){
        fprintf(stderr,"(parent_process>created process with id: %d)\n", pid);
    }
    if (pid == 0) /*child1*/
    {
        if(debug == 0){
            fprintf(stderr,"(child1>redirecting stdout to the write end of the pipe…)\n");
        }
        fclose(stdout);  /*3.1*/
        dup(pipefd[1]); /*3.2*/
        close(pipefd[1]); /*3.3*/
        if(debug == 0){
           fprintf(stderr,"(child1>going to execute cmd: …)\n");
        }
        if(execvp(argumentsLs[0], argumentsLs) < 0){ /*3.4*/
            perror("error execvp in child1 fails");
            _Exit(1);
        }
    }
    else /*parent*/
    {
        if(debug == 0){
            fprintf(stderr,"(parent_process>closing the write end of the pipe…)\n");
        }
        close(pipefd[1]); /*4*/
    }
    if(debug == 0){
        fprintf(stderr,"(parent_process>forking…)\n");
    }
    pid2 = fork(); /*5*/
    if(debug == 0){
        fprintf(stderr,"(parent_process>created process with id: %d)\n", pid);
    }
    if (pid2 == 0) /*child2*/
    {
        if(debug == 0){
            fprintf(stderr,"(child2>redirecting stdin to the read end of the pipe…)\n");
        }
        fclose(stdin); /*6.1*/
        dup(pipefd[0]); /*6.2*/
        close(pipefd[0]); /*6.3*/
        if(debug == 0){
            fprintf(stderr,"(child2>going to execute cmd: …)\n");
        }
        if(execvp(argumentsTail[0], argumentsTail) < 0){ /*6.4*/
            perror("error execvp in child2 fails");
            _Exit(1);
        }
    }
    else /*parent*/
    {
        if(debug == 0){
            fprintf(stderr,"(parent_process>closing the read end of the pipe…)\n");
        }
        close(pipefd[0]); /*7*/
    }
    if(debug == 0){
        fprintf(stderr,"(parent_process>waiting for child processes to terminate…)\n");
    }
    waitpid(pid, 0, 0); /*8*/
    waitpid(pid2, 0, 0);
    if(debug == 0){
        fprintf(stderr,"(parent_process>exiting…)\n");
    }
    return 0;
}
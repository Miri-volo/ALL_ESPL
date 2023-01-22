#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char debug_mode;
  char file_name[128];
  int unit_size;
  unsigned char mem_buf[10000];
  size_t mem_count;
} state;

struct fun_desc {
  char *name;
  void (*fun)(state*);
};

void displays_a_menu(struct fun_desc *menu){
  int i=0;
  fprintf(stderr,"Choose actoin:\n");
  while(menu[i].name != NULL){
    fprintf(stderr,"%i-%s\n",i, menu[i].name);
    i++;
  }
}

void toggleDebugMode(state* s){
    if ((*s).debug_mode == 1)
    {
        (*s).debug_mode = 0;
        fprintf(stderr,"Debug flag now off\n");
    }
    else if ((*s).debug_mode == 0)
    {
        (*s).debug_mode = 1;
        fprintf(stderr,"Debug flag now on\n");
    }
}

void setFileName(state* s){
    char fileNameHelper[100];
    char fileName[100];
    fprintf(stderr,"Enter new file name: \n");
    fgets(fileNameHelper, sizeof(fileNameHelper), stdin);
    sscanf(fileNameHelper, "%s", fileName);
    strcpy((*s).file_name, fileName);
    if ((*s).debug_mode == 1)
    {
        fprintf(stderr,"Debug: file name set to %s\n", fileName);
    }
}

void setUnitSize(state* s){
    char input[10];
    int number;
    char numberChar;
    fprintf(stderr,"Enter number: \n");
    fgets(input,sizeof(input),stdin);
    sscanf(input, "%c", &numberChar);
    number = (int)numberChar-48;
    if (number ==1 || number==2 || number==4) 
    {
        (*s).unit_size = number;
        fprintf(stderr, "Debug: set size to %d\n", number);
    }
    else{
        fprintf(stderr, "Wrong number %d\n", number);
    }
}

void loadIntoMemory(state* s){
    fprintf(stderr,"Not implemented yet\n");
}

void toggleDisplayMode(state* s){
    fprintf(stderr,"Not implemented yet\n");
}

void memoryDisplay(state* s){
    fprintf(stderr,"Not implemented yet\n");
}

void saveIntoFile(state* s){
    fprintf(stderr,"Not implemented yet\n");
}

void memoryModify(state* s){
    fprintf(stderr,"Not implemented yet\n");
}

void quit(state* s){
    if ((*s).debug_mode == 1)
    {
        fprintf(stderr,"quitting\n");
    }
    exit(0);
}


int main(int argc, char **argv)
{
    state *s = malloc(sizeof(state));
    struct fun_desc menu[] = {
        {"Toggle Debug Mode", toggleDebugMode},
        {"Set File Name", setFileName},
        {"Set Unit Size", setUnitSize},
        {"Load Into Memory", loadIntoMemory},
        {"Toggle Display Mode", toggleDisplayMode},
        {"Memory Display", memoryDisplay},
        {"Save Into File", saveIntoFile},
        {"Memory Modify", memoryModify},
        {"Quit", quit},
        { NULL, NULL }};
    char input;
    char tempInput[10];
    int sizeOfmenu = sizeof(menu)/8 - 1;  //8 bytes, because 4 for char
    while(1){
        if(s!=NULL && (*s).debug_mode==1){
            fprintf(stderr,"Unit size: %d\n", (*s).unit_size);
            fprintf(stderr,"File name: %s\n", (*s).file_name);
            fprintf(stderr,"Mem count: %d\n", (*s).mem_count);
        }
        displays_a_menu(menu);
        fgets(tempInput, sizeof(tempInput), stdin);
        sscanf(tempInput, "%c", &input);
        if((int)input-48>=0 && (int)input-48 <sizeOfmenu){
            fprintf(stderr,"Option : %i\n", (int)input-48);
            fprintf(stderr,"Within bounds\n");
            if(s!=NULL){
                menu[(int)input-48].fun(s);
            }
            fprintf(stderr,"DONE.\n");
        }
        else{
            fprintf(stderr,"Not within bounds\n");
            exit(0);
        }
    }
    return 0;
}

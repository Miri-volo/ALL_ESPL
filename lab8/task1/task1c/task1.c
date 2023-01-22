#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef struct {
  int display_mode;
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
    FILE *file;
    char input[10000];
    int location;
    int length;
    if (strcmp((*s).file_name, "") == 0)
    {
        perror("Error: empty file name\n");
        return;
    }
    file = fopen((*s).file_name, "r");
    if(file == NULL)
    {
        perror("Error: reading file\n");
        return;
    }
    fprintf(stderr,"Please enter <location> <length>\n");
    fgets(input,sizeof(input),stdin);
    sscanf(input, "%X%d", &location , &length);
    if ((*s).debug_mode == 1)
    {
        fprintf(stderr, "Debug: file name: %s\n", (*s).file_name);
        fprintf(stderr, "Debug: location: %X\n", location);
        fprintf(stderr, "Debug: length: %d\n", length);
    }
    fseek(file, location, SEEK_SET); //start from location
    int bytesRead = fread(s->mem_buf, sizeof(char), s->unit_size * length, file);
    s->mem_count = bytesRead;
    fprintf(stderr, "Loaded %d bytes into memory\n", bytesRead);
    fclose(file);
}

void toggleDisplayMode(state* s){
    if (s->display_mode == 0) {
        s->display_mode = 1;
        printf("Display flag now on, hexadecimal representation.\n");
    } else {
        s->display_mode = 0;
        printf("Display flag now off, decimal representation.\n");
    }
}

void memoryDisplay(state* s){
    char input[10000];
    int addr;
    int rows;
    printf("Enter address and length:\n");
    fgets(input,sizeof(input),stdin);
    sscanf(input, "%X%d", &addr ,&rows);
    if (s->display_mode == 0) {
        printf("Decimal\n=======\n");
        unsigned char* current = s->mem_buf + (addr * s->unit_size);
        unsigned char* rowsEnd = s->mem_buf + (rows * s->unit_size);
        unsigned char* end = s->mem_buf + s->mem_count;
        while (current < end && current < rowsEnd) {
            if (s->unit_size == 1) {
                printf("%hhd\n", *current);
            } else if (s->unit_size == 2) {
                printf("%hd\n", *current);
            } else {
                printf("%ld\n", *((long*) current));
            }
            current += s->unit_size;
        }
    } else {
        printf("Hexadecimal\n==========\n");
        unsigned char* current = s->mem_buf + (addr * s->unit_size);
        unsigned char* rowsEnd = s->mem_buf + (rows * s->unit_size);
        unsigned char* end = s->mem_buf + s->mem_count;
        while (current < end && current < rowsEnd) {
            if (s->unit_size == 1) {
                printf("%hhx\n", *current);
            } else if (s->unit_size == 2) {
                printf("%hx\n", *current);
            } else {
                printf("%lx\n", *((long*) current));
            }
            current += s->unit_size;
        }
    }
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
    s->unit_size = 2;
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
            printf("Option : %i\n", (int)input-48);
            printf("Within bounds\n");
            if(s!=NULL){
                menu[(int)input-48].fun(s);
            }
            printf("DONE.\n");
        }
        else{
            printf("Not within bounds\n");
            exit(0);
        }
    }
    return 0;
}

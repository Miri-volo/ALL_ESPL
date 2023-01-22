#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

typedef struct link link;
struct link {
    link *nextVirus;
    virus *vir;
};

struct fun_desc {
  char *name;
  link * (*fun)(link *);
};

void PrintHex(unsigned char* buffer,int length, FILE* output)
{
    for(int i=0;i<length;i++)
        fprintf(output, "%hhX ",buffer[i]);
    fprintf(output, "\n");
}

virus* readVirus(FILE* file){
    virus* v = (virus*)malloc(sizeof(virus));
    fread(v, sizeof(char), 18, file);
    (*v).sig = (unsigned char*) malloc(sizeof(char)*(*v).SigSize);
    fread((*v).sig, sizeof(char), (*v).SigSize, file);
    return v;
}

void printVirus(virus* virus, FILE* output){
    fprintf(output, "Virus name: %s\nVirus size: %d\nsignature:\n", (*virus).virusName, (*virus).SigSize);
    PrintHex((*virus).sig, (*virus).SigSize, output);
}

void list_print(link *virus_list, FILE* file){
    if(virus_list != NULL && (*virus_list).vir != NULL){
        printVirus((*virus_list).vir, file);
        list_print((*virus_list).nextVirus, file);
    }
}

link* list_append(link* virus_list, virus* data){
    link* l;
    if((*virus_list).vir == NULL){
        (*virus_list).vir = data;
        return virus_list;
    }
    if((*virus_list).nextVirus == NULL){
        l =(link*)malloc(sizeof(link));
        (*l).vir = data;
        (*virus_list).nextVirus = l;
    }
    else
        list_append((*virus_list).nextVirus, data);
    return virus_list;
}

void virus_free(virus* v){
    if(v!=NULL){
        free((*v).sig);
        free(v);
    }
}

void list_free(link *virus_list){
    if(virus_list!= NULL){
        virus_free((*virus_list).vir);
        list_free((*virus_list).nextVirus);
        free(virus_list);
    }
}


void detect_virus(char *buffer, unsigned int size, link *virus_list){
    int sigSize;
    unsigned char* sig;
    if((*virus_list).vir != NULL){
        sigSize = (*virus_list).vir -> SigSize;
        sig =(*virus_list).vir -> sig;
        if(size < sigSize)
            detect_virus(buffer, size, (*virus_list).nextVirus);
        for(int i = 0; i < size - sigSize; i++){
            if(memcmp(buffer + i, sig, sigSize) == 0){
                printf("Starting byte location in the suspected file: %d\n", i);
                printf("Virus name: %s\n", (*(*virus_list).vir).virusName);
                printf("Size of the virus signature: %i\n", sigSize);
            }
        }
        if((*virus_list).nextVirus != NULL)
            detect_virus(buffer, size, (*virus_list).nextVirus);
    }
}

link * call_detect_virus(link* list){
    char buffer[10000];
    FILE* file;
    char fname[50];
    char fnameHelper[50];
    int sizeToRead;
    printf("Please enter file name: \n");
    fgets(fnameHelper, sizeof(fnameHelper), stdin);
    sscanf(fnameHelper, "%s", fname);
	file = fopen(fname,"r");
    if(file == NULL) {
        perror("File cannot be opened.\n");
        exit(0);
    }
    fseek(file, 4, SEEK_END);
    sizeToRead = ftell(file);
    fseek(file, 4, SEEK_SET);
    fread(buffer, 1, sizeToRead, file);
    fclose(file);
    detect_virus(buffer, sizeToRead, list);
    return list;
}

void fix_file(FILE* file){
    printf("Not implemented\n");
}

link * call_fix_file(link* list){
    printf("Not implemented\n");
    return list;
}

link * quit(link *virus_list){
    if(virus_list!=NULL){
        list_free(virus_list);
        free(virus_list)
    }
    exit(0);
}


void Displays_a_menu(struct fun_desc *menu){
  printf("Please choose a function:\n");
  int i=0;
  while(menu[i].name != NULL){
    printf("%i) %s\n",i, menu[i].name);
    i++;
  }
}

link * print_signatures(link *list){
    FILE* file;
    char fname[50];
    char fnameHelper[50];
    printf("Please enter output file name: \n");
    fgets(fnameHelper, sizeof(fnameHelper), stdin);
    sscanf(fnameHelper, "%s", fname);
    file = fopen(fname,"w+");
    if(list!=NULL)
        list_print(list, file);
    fclose(file);
    return list;
}

link * load_signatures(link *list){
    FILE* file;
    char fname[50];
    char fnameHelper[50];
    virus* v = (virus*)malloc(sizeof(virus)); 
    printf("Please enter signature file name: \n");
    if(list != NULL)
        list_free(list);
    fgets(fnameHelper, sizeof(fnameHelper), stdin);
    sscanf(fnameHelper, "%s", fname);
	file = fopen(fname,"r");
    if(file == NULL) {
        perror("File cannot be opened.\n");
        exit(0);
    }
    list = (link*)malloc(sizeof(link));
    while(fgetc(file) != EOF){
        fseek(file, -1, SEEK_CUR);
        v = readVirus(file);
        list = list_append(list, v);
    }
    fclose(file);
    return list;
}

int main(int argc, char **argv){
  char input;
  char tempInput[10];
  link *list = NULL;
  struct fun_desc menu[] = { { "Load signatures", load_signatures}, { "Print signatures", print_signatures}, { "Detect viruses", call_detect_virus }, { "Fix file", call_fix_file}, { "Quit", quit} ,{ NULL, NULL } };
  int sizeOfmenu = sizeof(menu)/8 - 1; //8 bytes, because 4 for char
  Displays_a_menu(menu);
  while(1){
    fgets(tempInput, sizeof(tempInput), stdin);
    sscanf(tempInput, "%c", &input);
    if((int)input-48>=0 && (int)input-48 <sizeOfmenu){
      printf("Option : %i\n", (int)input-48);
      printf("Within bounds\n");
      list = menu[(int)input-48].fun(list);
      printf("DONE.\n");
      Displays_a_menu(menu);
    }
    else{
      printf("Not within bounds\n");
      list_free(list);
      free(list);
      exit(0);
    }
  }
  return 0;
}
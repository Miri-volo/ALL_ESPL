#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
char censor(char c) {
  if(c == '!')
    return '.';
  else
    return c;
}
 
char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  /* TODO: Complete during task 2.a */
  for(int i=0; i<array_length; i++){
    mapped_array[i] = (*f)(array[i]);
  }
  strcpy(array, mapped_array);
  free(mapped_array);
  return array;
}

 char my_get(char c){
/* Ignores c, reads and returns a character 
    from stdin using fgetc. */
    return fgetc(stdin);
 }
char cprt(char c){
/* If c is a number between 0x20 and 0x7E,
 cprt prints the character of ASCII value c 
 followed by a new line. Otherwise, 
 cprt prints the dot ('.') character. 
 After printing, cprt returns the value of c unchanged. */
    if((int)c <=126 && (int)c>=32)
        printf("%c\n",c);
    else
        printf(".\n");
    return c;
}
char encrypt(char c){
/* Gets a char c and returns its encrypted form by adding 3
 to its value. If c is not between 0x20 and 0x7E it is 
 returned unchanged */
    if((int)c <=126 && (int)c>=32)
        return c+3;
    else
        return c;
}

char decrypt(char c){
/* Gets a char c and returns its decrypted form by 
reducing 3 to its value. If c is not between 0x20 and 0x7E
 it is returned unchanged */
    if((int)c <=126 && (int)c>=32)
        return c-3;
    else
        return c;
}

char xprt(char c){
/* xprt prints the value of c in a hexadecimal representation
 followed by a new line, and returns c unchanged. */
    printf("%X\n", c);
    return c;
}
char quit(char c){
/* Gets a char c, and if the char is 'q' , 
ends the program with exit code 0. Otherwise returns c. */
    if(c=='q')
        exit(0);
    return c;
}

struct fun_desc {
  char *name;
  char (*fun)(char);
}; 

void Displays_a_menu(struct fun_desc *menu){
  printf("\nPlease choose a function:\n");
  int i=0;
  while(menu[i].name != NULL){
    printf("%i) %s\n",i, menu[i].name);
    i++;
  }
}

int main(int argc, char **argv){
  char input;
  char * carray = (char*) malloc(5 * sizeof(char));
  carray[0] = '\0'; 
  struct fun_desc menu[] = { { "Get string", my_get }, { "Print string", cprt}, { "Print hex", xprt }, { "Censor", censor}, { "Encrypt", encrypt}, { "Decrypt", decrypt}, {"Quit",quit},{ NULL, NULL } };
  Displays_a_menu(menu);
  int sizeOfmenu = sizeof(menu)/8 - 1; //8 bytes, because 4 for char
  while ((input = fgetc(stdin)) != EOF){
    if((int)input-48>=0 && (int)input-48 <sizeOfmenu){
      printf("Option : %i\n", (int)input-48);
      printf("Within bounds\n");
      map(carray, 5, menu[((int)input-48)].fun);
      printf("DONE.\n");
    }
    else if(input=='\n'){
      Displays_a_menu(menu);
      continue;
    }
    else{
      printf("Not within bounds\n");
      exit(0);
    }
  }
  free(carray);
  return 0;
}
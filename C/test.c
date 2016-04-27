#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

char** split(char* str, char delim) {
  int i = 0;
  int count_space = 0;
  char* s = malloc(sizeof(char)*strlen(str));
  for (i = 0; i < strlen(str); i++)
  {
    s[i]=str[i];
  }
  char* d = &delim;
  for (i = 0; i < strlen(str); i++) {
    if(str[i] == delim){
      count_space++;
    }
  }
  printf("Value of count_space %d\n",count_space);
  if(count_space >0){
    count_space += 2;
  }
  //printf("Val of count_space %d\n",count_space);
  char** tab = malloc(sizeof(char*)*(count_space));
  char* p = strtok(s,d);
  int j = 0;
  while(p!=NULL){
    tab[j] = malloc(sizeof(char)*strlen(p));
    strcpy(tab[j],p);
    printf("value de tab[j] dans strtok %s\n",tab[j]);
    j++;
    p = strtok(NULL,d);
  }
  tab[count_space-1]=NULL;
  return tab;
}


int main(int argc, char *argv[]){
  char** tab = split("WHOS 12345678",' ');
  printf("Tab %s\n",tab[1]);
  return 0;
}

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



int main(int argc, char *argv[]){
  /*
  int sock = socket(PF_INET,SOCK_DGRAM,0);
  struct addrinfo *finfo;
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  int r = getaddrinfo("127.000.000.001","50000",&hints,&finfo);
  if (r==0) {
    if (finfo!=NULL){
      struct sockaddr *saddr=finfo->ai_addr;
      char tampon[100];
      int i;
      for (i = 0; i <= 10; i++) {
        strcpy(tampon,"Premiers test UDP ");
        char entier[3];
        sprintf(entier,"%d",i);
        strcat(tampon,entier);
        sendto(sock,tampon,strlen(tampon),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
      }
    }
  }*/
  /* time_t t; */
  /* char* tmp = malloc(sizeof(char)*7); */
  /* struct timespec start; */
  /* int r = clock_gettime(CLOCK_REALTIME,&start); */
  /* if(r==0){ */
  /*   char* idm = malloc(sizeof(char)*8); */
  /*   char* tab = "azertyuiopqsdfghjklmwxcvbn"; */
  /*   srand((unsigned) time(&t)); */
  /*   idm[0] = tab[rand()%26]; */
  /*   int i = (int)start.tv_nsec; */
  /*   sprintf(tmp,"%d",i); */
  /*   strcat(idm,tmp); */
  /*   printf("Value of imd %s\n",idm); */
  /* } */
  int r=0;
  char* ip = "127.000.000.001";
  int i;
  char* test = malloc(sizeof(char)*9);
  if(strlen(ip)==15){
    for (i = 0; i < 4; i++)
    {
      test[i]=ip[i];
    }
    test[4]=ip[6];
    test[5]=ip[7];
    test[6]=ip[10];
    test[7]=ip[11];
    test[8]=ip[14];
  }else{
    test = ip;
  }
  struct sockaddr_in sa;
  int result = inet_pton(AF_INET, test, &(sa.sin_addr));
  if(result!=0) r = 1;
  else{
    result = inet_pton(AF_INET6, test, &(sa.sin_addr));
    if(result != 0) r = 1;
  }
  printf("Value of r %d\n",r);
  return 0;
}

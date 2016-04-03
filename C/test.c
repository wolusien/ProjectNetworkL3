#include <stdio.h>
#include <stdlib.h>
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
  strncpy(s,str,strlen(str));
  char* d = &delim;
  for (i = 0; i < strlen(str); i++) {
    if(str[i] == delim){
      count_space++;
    }
  }
  if(count_space >0){
    count_space++;
  }
  printf("Val of count_space %d\n",count_space);
  char** tab = malloc(sizeof(char*)*(count_space));
  char* p = strtok(s,d);
  int j = 0;
  while(p!=NULL){
    tab[j] = malloc(sizeof(char)*strlen(p));
    strcpy(tab[j],p);
    j++;
    p = strtok(NULL,d);
  }
  for (i = 0; i < count_space; i++) {
    printf("Split : Val of tab[%d] %s\n",i,tab[i]);
  }
  return tab;
}

int str_arrsize(char** tab){
  int size = 0;
  while(tab[size]!=NULL){
    size++;
  }
  return size;
}

int insertion(entity* e1, char* host, char* e1_tcp ){
  
  int sock = socket(PF_INET,SOCK_STREAM,0);
  struct sockaddr_in* adressin;
  struct addrinfo* finfo;
  struct addrinfo hints;
  bzero(&hints,sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  int r = getaddrinfo(host,e1_tcp, &hints, &finfo);
  if(r==0){
    if(finfo!=NULL){
      adressin = (struct sockaddr_in*)finfo->ai_addr;
      int con = connect(sock,(struct sockaddr*)adressin,(socklen_t)sizeof(struct sockaddr_in));
      if(con == 0){
        char buff[100];
        int recu =  recv(sock,buff,99*sizeof(char),0);
        if(recu>0){
          buff[recu]='\0';
          char** tab = split(buff,' ');
          if (str_arrsize(tab)==5) {
            char* ip_e2 = tab[1];
            char* port_e2 = tab[2];
            char* ip_cast = tab[3];
            char* port_cast = tab[4];
            (*e1).next_ip = ip_e2;
            (*e1).next_uport = port_e2;
            (*e1).cast_ip = ip_cast;
            (*e1).cast_port = cast_port;
            char* mess = "NEWC ";
            strcat(mess, (*e1).my_ip);
            strcat(mess, " ");
            strcat(mess, (*e1).my_uport);
            strcat(mess, "\n");
            int send = send(sock,mess, strlen(mess)*sizeof(char));
            if (send == 0) {
              char buff2[100];
              int recu2 =  recv(sock,buff2,99*sizeof(char),0);
              if(recu2>0){
                buff2[recu2] = '\0';
                if(strcmp(buff2,"ACKC") == 0){
                  close(sock);
                  free(tab);
                  return 0;
                }else {
                  printf("recv : buff2 Problem with  message receive %s",buff2);
                  return -1;
                }
              }
            }else {
              printf("send : Problem with sending informations message");
              return -1;
            }
          }else{
            printf("recv : Problem with the message receive");
            return -1;
          }
        }
      }else{
        printf("connect : Problem with the connection %d\n" ,con);
        return -1;
      }
    }else{
      printf("getaddrinfo : Problem struct addrinfo* finfo NULL\n");
      return -1;
    }
  }else{
    printf("getaddrinfo : Problem %d\n" ,r);
    return -1;
  }
  
}

int main(int argc, char *argv[]) {
  char *str = "127.0.0.1 490000 127.0.0.1 49000";
  char** tab = split(str,' ');
  int i = 0;
  for (i = 0; i < 4; i++) {
    printf("Main : Value of tab[%d] %s\n",i,tab[i]);
  }
  return 0;
}


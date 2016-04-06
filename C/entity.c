#include "entity.h"


/*!Function which initialize an entity
 *\param char id[8] id of the entity
 *\param int my_uport udp port of the entity
 *\param int tcp_port  tcp port of the enity
 *\param char* ip_ad ip address of the entity
 *\param int next_uport udp por of the next entity
 *\param char* ipv4_ad ipv4 adress of broadcast
 *\param int cast_port tcp port of broadcast
 *\return entity* pointer to an entity created
 */
int init_entity(entity* ent, char* id, char* my_ip, int my_uport, int tcp_port, int prev_uport, char* next_ip, int next_uport, char* cast_ip, int cast_port){
  if(id!=NULL && next_ip!=NULL && cast_ip!=NULL
     && strlen(next_ip)==15 && strlen(cast_ip)==15){
    if(my_uport>9999 && my_uport<65535 
       && tcp_port>9999 && tcp_port<65535
       && next_uport>9999 && next_uport<65535
       && prev_uport>9999 && prev_uport<65535
       && cast_port>9999 && cast_port<65535){
      //(*ent).id = id;
      (*ent).my_ip = malloc(sizeof(char)*strlen(my_ip));
      (*ent).my_ip = my_ip;
      (*ent).my_uport = my_uport;
      (*ent).tcp_port = tcp_port;
      (*ent).prev_uport = prev_uport;
      (*ent).next_ip = malloc(sizeof(char)*strlen(next_ip));
      (*ent).next_ip =  next_ip;
      (*ent).next_uport = next_uport;
      (*ent).cast_ip = malloc(sizeof(char)*strlen(cast_ip));
      (*ent).cast_ip = cast_ip;
      (*ent).cast_port = cast_port;
      return 0;
    }
  }
  return -1;
}

/*
 Function that handle the insertion
 */
int insertion(entity* e1, char* host, int e1_tcp ){
  //Settings structures needed later
  int sock = socket(PF_INET,SOCK_STREAM,0);
  struct sockaddr_in* adressin;
  struct addrinfo* finfo;
  struct addrinfo hints;
  bzero(&hints,sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  char* tcp_e1 = malloc(sizeof(char)*6);
  sprintf(tcp_e1,"%d",e1_tcp);
  //Getting information about previous entity
  int r = getaddrinfo(host,tcp_e1, &hints, &finfo);
  if(r==0){
    if(finfo!=NULL){
      
      //Getting address of the previous entity
      adressin = (struct sockaddr_in*)finfo->ai_addr;
      int con = connect(sock,(struct sockaddr*)adressin,(socklen_t)sizeof(struct sockaddr_in));
      
      if(con == 0){
        char buff[100];
        int recu =  recv(sock,buff,99*sizeof(char),0);
        if(recu>0){
          //Managing the message received from the previous entity [WELC␣ip␣port␣ip-diff␣port-diff\n]
          buff[recu]='\0';
          printf("Message received from entity1 %s\n" ,buff);
          char** tab = split(buff,' ');
          if (str_arrsize(tab)==5) {
            char* ip_e2 = tab[1];
            int port_e2 = atoi(tab[2]);
            char* ip_cast = tab[3];
            int port_cast = atoi(tab[4]);
            //Allocation to current entity of the information received from the previous entity
            (*e1).next_ip = ip_e2;
            (*e1).next_uport = port_e2;
            (*e1).cast_ip = ip_cast;
            (*e1).cast_port = port_cast;
            //Preparation of the current entity answer [NEWC␣ip␣port\n]
            char mess[120];
            strcpy(mess,"NEWC ");
            char my_ip[strlen((*e1).my_ip)];
            strcpy(my_ip,(*e1).my_ip);
            strcat(mess, my_ip);
            strcat(mess, " ");
            char* my_port = malloc(sizeof(char)*7);
            sprintf(my_port,"%d",(*e1).my_uport);
            strcat(mess, my_port);
            strcat(mess, "\n");
            //Sending to the previous entity the message about current entity
            send(sock,mess, strlen(mess)*sizeof(char),0);
            char buff2[100];
            int recu2 =  recv(sock,buff2,99*sizeof(char),0);
            if(recu2>0){
              buff2[recu2] = '\0';
              printf("Value of the message received from entity 1 %s" ,buff2);
              if(strcmp(buff2,"ACKC\n") == 0){
                close(sock);
                free(tab);
                return 0;
              }else {
                printf("recv : buff2 Problem with  message receive %s",buff2);
                return -1;
              }
            }else{
              printf("recv : Problem with the message receive");
              return -1;
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
  }
  return -1;
}

#include "entity.h"


/*
Function which initialize an entity
 */
int init_entity(entity* ent, char* host){
  char id[8];
  strcpy(id,gen_code());
  char* ip = get_ip(host);
  int uport = free_uport(host);
  int tport = free_tport(host);
  if(ip != NULL && uport != -1 && tport != -1){
    (*ent).id = id;
    (*ent).my_ip = ip;
    (*ent).my_uport = uport;
    (*ent).tcp_port = tport;
    return 0;
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
            if(check_ip(ip_e2) != -1 && check_ip(ip_cast) != -1
               && port_e2>=9999 && port_e2<65535
               && port_cast>=9999 && port_cast<65535
               && port_cast != port_e2
               && strcmp(ip_cast, ip_e2)!=0){
            
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
                printf("Value of the message received from entity 1 %s\n" ,buff2);
                if(strcmp(buff2,"ACKC\n") == 0){
                  close(sock);
                  free(tab);
                  return 0;
                }else {
                  printf("recv : buff2 Problem with  message receive %s\n",buff2);
                  return -1;
                }
              }else{
                printf("recv : Problem with the message receive\n");
                return -1;
              }
            }else{
                printf("recv : Problem with the message receive, wrong arguments\n");
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

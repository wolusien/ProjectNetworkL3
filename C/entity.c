#include "entity.h"

/*
Function which initialize an entity
 */
int init_entity(entity* ent, char* host){
  char id[8];
  strcpy(id,gen_code());
  int uport = free_uport(host);
  if(uport != -1){
    (*ent).id = id;
    (*ent).my_uport = uport;
    return 0;
  }
  return -1;
}

/*
 Function that handle the insertion
 */
int insertion(entity* e, char* host, int e1_tcp ){
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
            
              (*e).next_ip1 = ip_e2;
              (*e).next_uport1 = port_e2;
              (*e).cast_ip1 = ip_cast;
              (*e).cast_port1 = port_cast;
              //Preparation of the current entity answer [NEWC␣ip␣port\n]
              char mess[120];
              strcpy(mess,"NEWC ");
              char my_ip[strlen((*e).my_ip)];
              strcpy(my_ip,(*e).my_ip);
              strcat(mess, my_ip);
              strcat(mess, " ");
              char* my_port = malloc(sizeof(char)*7);
              sprintf(my_port,"%d",(*e).my_uport);
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
                  printf("recv : Problem with  message receive %s\n",buff2);
                  return -1;
                }
              }else{
                printf("recv : Problem with the second message received %d\n",recu2);
                return -1;
              }
            }else{
              printf("recv : Problem with the ip and ports received\n");
              return -1;
            }		
          }else{
            printf("recv : Problem wrong message form\n");
            return -1;
          }
        }else{
          printf("recv : Problem the number of char received %d\n",recu);
          return -1;
        }
      }else{
        printf("connect : Problem with the connection %d\n" ,con);
        return -1;
      } 
    }else{
      printf("getaddrinfo : Problem the struct addrinfo finfo is NULL\n");
      return -1;
    }
  }else{
    printf("getaddrinfo : Problem %d\n" ,r);
    return -1;
  }
  return -1;
}

/*
Function that manage insertion(server part)
*/
int serv_tcp(entity* e){
	
	int sock=socket(PF_INET,SOCK_STREAM,0);
  struct sockaddr_in address_sock;
  
  address_sock.sin_family = AF_INET;
  if((*e).tcp_port>9999 && (*e).tcp_port <=65535){
    address_sock.sin_port = htons((*e).tcp_port);
    inet_aton((*e).my_ip,&address_sock.sin_addr);

    int r=bind(sock,(struct sockaddr *)&address_sock,sizeof(struct sockaddr_in));

    if(r==0){
      r=listen(sock,0);
      while(1){
        struct sockaddr_in caller;
        socklen_t size=sizeof(caller);
        int sock2=accept(sock,(struct sockaddr *)&caller,&size);
        
        if(sock2>=0){
          //Preparation of the current entity answer [NEWC␣ip␣port\n]
          char mess[120];
          strcpy(mess,"WELC ");
          //Establishing ip(next_ip)
          char next_ip[strlen((*e).next_ip1)];
          strcpy(next_ip,(*e).next_ip1);
          strcat(mess, next_ip);
          strcat(mess, " ");
          //Establishing port(next_uport)
          char* next_uport = malloc(sizeof(char)*7);
          sprintf(next_uport,"%d",(*e).next_uport1);
          strcat(mess, next_uport);
          strcat(mess, " ");
          //Establishing ip_diff
          char cast_ip[strlen((*e).cast_ip1)];
          strcpy(cast_ip,(*e).cast_ip1);
          strcat(mess, cast_ip);
          strcat(mess, " ");
          //Establishing port_diff
          char* cast_port = malloc(sizeof(char)*7);
          sprintf(cast_port,"%d",(*e).cast_port1);
          strcat(mess, cast_port);
          strcat(mess, "\n");
          //Sending to the previous entity the message about current entity
          send(sock2,mess,strlen(mess)*sizeof(char),0);
          
          //Manage message reception
          char buff[100];
          int recu=recv(sock2,buff,99*sizeof(char),0);
          buff[recu]='\0';
          if (recu>0){
            char** tab = split(buff,' ');
            if (str_arrsize(tab)==3){
              if(strcmp(tab[0],"NEWC")==0){
                int next_uport = atoi(tab[2]);
                if(check_ip(tab[1]) != -1 
                && next_uport>9999 && next_uport<=65535){
                  (*e).next_ip1 = tab[1];
                  (*e).next_uport1 = next_uport;
                  printf("Message recu : %s\n",buff);
                  char* m = "ACKC\n";
                  send(sock2,m,strlen(m)*sizeof(char),0);
                  close(sock2);
                }else{            
                  printf("serv_tcp : Problem with the message received wrong arguments\n");
                  close(sock2);
                }
              }else{            
                printf("serv_tcp : Problem with the message received wrong arguments\n");
                close(sock2);
              }
            }else{            
              printf("serv_tcp : Problem with the message received wrong arguments\n");
              close(sock2);
            }
          }else{
            printf("serv_tcp : Problem with the message received wrong form\n");
            close(sock2);
          }
        }else{
          printf("serv_tcp : Problem with the connection to the server\n");
          close(sock2);
        }
      }
    }else{
      printf("serv_tcp : Problem with the bind of the server socket\n");
      return -1;    
    }  
  }else{
    printf("serv_tcp : Wrong tcp port given %d\n",(*e).tcp_port);
    return -1;
  }
  return 0;
}

/*Function for using thread for the insertion
 */
void* pth_insertion(void* arg){
  entity* e = (entity*)arg;
  char* host = (*e).my_ip;
  char* ip = get_ip(host);
  
  int tcp_port = (*e).tcp_port;
  int port = free_tport(host);
  if(ip!=NULL && port!=-1){
    (*e).my_ip = ip;
    (*e).tcp_port = port;
    insertion(e,host,tcp_port);
  }  
  return NULL;
}

/*Function for using thread for the server tcp
 */
 void* pth_tserv(void* arg){
  entity* e = (entity*)arg; 
  serv_tcp(e);
  return NULL; 
 }

#include "udp.h"

/* Function initialising structure app_message */
int init_appmess(app_message* m, char* id_app, char* mess){
  if (strlen(id_app)==8 && strlen(mess)==512) {
    (*m).idm = gen_idmess();
    strcpy((*m).id_app,id_app);
    strcpy((*m).message_app,mess);
    return 0;
  }else{
    return -1;
  }
}

int init_uEntity(uEntity* u, entity* e){
  if(ent!=NULL){
    (*u).ent = e;
    return 0; 
  }
  return -1;
} 

int isin(uEntity* u, int ptr){
  int r = 0;
  for (i = 0; i < (*u).rec_pos; i++){
    if((*u).rec[i]==ptr){
      r=-1;
      break;
    }
  }
  return 0;
} 


int rec_udp(uEntity* u){
  int sock = socket(PF_INET,SOCK_DGRAM,0);
  struct sockaddr_in address_sock;
  address_sock.sin_family = AF_INET;
  if((*u).(*e).my_uport>9999 && (*u).(*e).my_uport<=65535){
    address_sock.sin_port = htons((*u).(*e).my_uport);
    inet_aton((*e).my_ip,&address_sock.sin_addr);
    
    int r = bind(sock,(struct sockaddr*)&address_sock,sizeof(struct sockaddr_in));
    if (r==0) {
      char buff[512];
      while (1) {
        int rec = recv(sock,buff,512,0);
        if(rec>0){
          buff[rec]='\0';
          /*reste à coder */

        }
      }
    }else {
      fprintf(stderr,"rec_udp bind: Problem with socket binding %d\n",r);
      return -1;
    }
  }else{
    fprintf(stderr,"rec_udp : Wrong udp port entity\n");
  }
  return -1;
}

int utags_message(char* buff, uEntity* u){
  if(app_mess(buff,u)!=-1){
    int r = app_mess(buff,u);
    if(r==0){
      return 0;
    }
    fprintf(stderr," utags_message : Problem with application message\n");
  }else if()
  return -1;
}

int app_mess(char* buff, uEntity* u){
  if(strlen(buff)==512){
    int sock = socket(PF_INET,SOCK_DGRAM,0);
    struct sockaddrinfo *finfo;
    struct addrinfo hints;
    
    bzero(&hints,sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    
    char* next_uport = malloc(sizeof(char)*100);
    sprintf(next_uport,"%d",(*(u->e)).next_uport1);
		
    int r = getaddrinfo((*(u->e)).next_ip1,next_port,&hints,&finfo);
    char** tab = split(buff,' ');
    if(str_arrsize(tab)==4){
      if (strcmp(tab[0],"APPL")==0) {
        if (strlen(tab[2])==8) {
          if(strcmp((*u).id_app,tab[2])==0){
            //Case where (*u).(*e) support the application
            //code à écrire
            return 0;
          }else{
            //Case where we transmit the message to the next entity
            if(r==0){
              if(finfo!=NULL){
                struct sockaddr *saddr = finfo->ai_addr;
                sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
                return 0;
              }else{
                fprintf(stderr,"app_mess : Problem with getaddrinfo : struct sockaddrinfo finfo is NULL\n");
                return 1;
              }
            }else{
              fprintf(stderr,"app_mess : Problem with getaddrinfo %d\n",r);
              return 2;
            }
          }
        }else{
          fprintf(stderr,"app_mess ; Wrong application id %s\n",tab[2]);
        }
      }    
    }
  }
  return -1;
}


int whos(char* buff, uEntity* u){
  if(strlen(buff)==13){
    int sock = socket(PF_INET,SOCK_DGRAM,0);
    struct sockaddrinfo *finfo;
    struct addrinfo hints;
    
    bzero(&hints,sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    
    char* next_uport = malloc(sizeof(char)*100);
    sprintf(next_uport,"%d",(*(u->e)).next_uport1);
		
    int r = getaddrinfo((*(u->e)).next_ip1,next_port,&hints,&finfo);
    char** tab = split(buff,' ');
    if(str_arrsize(tab)==2){
      if (strcmp(tab[0],"WHOS")==0) {
        if (strlen(tab[1])==8) {
          if(isin(u,tab[1])==0){
            char* tampon = malloc(sizeof(char)*120);
            strcat(tampon,"MEMB ");
            strcat(tampon,tab[1]);
            strcat(tampon," ");
            strcat(tampon,(*u).e->id);
            strcat(tampon," ");
            strcat(tampon,(*u).e->my_ip);
            strcat(tampon," ");
            char* udp = intchar((*u).e->my_uport,4);
            if(udp != NULL){
              strcat(tampon,udp);
              sendto(sock,tampon,strlen(tampon),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
              return 0;
            }
          }
        }else{
          fprintf(stderr,"app_mess ; Wrong application id %s\n",tab[2]);
        }
      }    
    }
  }
  return -1;
}

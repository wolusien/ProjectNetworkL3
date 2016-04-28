#include "udp.h"

/*Function adding a char* mess to an array of an uEntity
 * depends on option given : 0 for env array
 *                           1 for rec array
 */
int add_umess(uEntity* u, int option, char* mess){
  //0 for env array
  //1 for rec array
  if(option == 0){
    if((*u).env_pos<(*u).env_size){
      (*u).env[(*u).env_pos]=mess;
      (*u).env_pos++;
      (*u).env[(*u).env_pos]=NULL;
      return 0;
    }else{
      (*u).env = realloc((*u).env,sizeof(char)*((*u).env_size+10));
      (*u).env_size += 10;
      (*u).env[(*u).env_pos]=mess;
      (*u).env_pos++;
      (*u).env[(*u).env_pos]=NULL;
      return 0;
    }
  }else if(option == 1){
    if((*u).rec_pos<(*u).rec_size){
      (*u).rec[(*u).rec_pos]=mess;
      (*u).rec_pos++;
      (*u).rec[(*u).rec_pos]=NULL;
      return 0;
    }else{
      (*u).rec = realloc((*u).rec,sizeof(char)*((*u).rec_size+10));
      (*u).rec_size += 10;
      (*u).rec[(*u).rec_pos]=mess;
      (*u).rec_pos++;
      (*u).rec[(*u).rec_pos]=NULL;
      return 0;
    }
  }
  return -1;
}

/*Test if a message is on array env then return 0
 * on array rec then return 1
 * else return -1
 */
int isin(uEntity* u, char* idm){
  int i;
  for (i = 0; i <(*u).env_pos ; i++)
  {
    if(strcmp((*u).env[i],idm)==0) return 0;
  }
  for (i = 0; i <(*u).rec_pos ; i++)
  {
    if(strcmp((*u).rec[i],idm)==0) return 1;
  }
  return -1;
} 


/* Function initialising structure app_message */
/*int init_appmess(app_message* m, char* id_app, char* mess){
  if (strlen(id_app)==8 && strlen(mess)==512) {
    (*m).idm = gen_idmess();
    strcpy((*m).id_app,id_app);
    strcpy((*m).message_app,mess);
    return 0;
  }else{
    return -1;
  }
}
*/

int init_uEntity(uEntity* u, char* host){
  entity* e = malloc(sizeof(entity));
  int r = init_entity(e,host);
  if(r==0){
    (*u).ent = e;
    init_entity((*u).ent,host);
    (*u).rec_size = (*u).env_size = 100;
    (*u).rec_pos = (*u).env_pos = 0;
    (*u).rec = malloc(sizeof(char*)*(*u).rec_size);
    (*u).env = malloc(sizeof(char*)*(*u).env_size);
    return 0; 
  }
  return -1;
} 


//////////////////////////////////////////////////////////////////////
///////////////////RECEPTION//////////////////////////////////////////

/*
 * Function managing application udp message protocol
 */
int app_mess(uEntity* u, char* buff){
  if(strlen(buff)<=512){
    int sock = socket(PF_INET,SOCK_DGRAM,0);
    struct addrinfo *finfo;
    struct addrinfo hints;
    
    bzero(&hints,sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    
    char* next_uport = malloc(sizeof(char)*100);
    sprintf(next_uport,"%d",(*(u->ent)).next_uport1);
		
    int r = getaddrinfo((*(u->ent)).next_ip1,next_uport,&hints,&finfo);    
    char** tab = split(buff,' ');
    if(str_arrsize(tab)>=4){
      if (strcmp(tab[0],"APPL")==0) {
        if (strlen(tab[1])==8)
        {
          if (isin(u,tab[1])==-1)
          {
            //Case where mess has never been managed
            if (strlen(tab[2])==8) {
              if(strcmp((*u).id_app,tab[2])==0){
                //Case where (*u).(*e) support the application
                //code à écrire
                printf("Code à écrire, message de l'application %s\n",buff);
                return 0;
              }else{
                //Case where we transmit the message to the next entity
                if(r==0){
                  if(finfo!=NULL){
                    struct sockaddr *saddr = finfo->ai_addr;
                    sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
                    
                    //Case for the second ring (duplication)
                    if((*u).ent->cast_ip2!=NULL && (*u).ent->next_ip2!=NULL && (*u).ent->cast_port2!=0 && (*u).ent->next_uport2!=0)
                    {    
                      sprintf(next_uport,"%d",(*(u->ent)).next_uport2);
                
                      r = getaddrinfo((*(u->ent)).next_ip2,next_uport,&hints,&finfo);    
                      if(r==0){
                        if(finfo!=NULL){
                          saddr = finfo->ai_addr;
                          sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
                          
                          add_umess(u,0,tab[0]);
                          add_umess(u,0,tab[1]);
                    
                          close(sock);
                          free(tab);
                          free(finfo);
                          return 0;
                        }else{
                          fprintf(stderr,"app_mess : Problem with getaddrinfo for second ring : struct sockaddrinfo finfo is NULL\n");
                          return -1;
                        }
                      }else{
                        fprintf(stderr,"app_mess : Problem with getaddrinfo for the second ring %d\n",r);
                        return -1;
                      }
                    }
                    add_umess(u,0,tab[0]);
                    add_umess(u,0,tab[1]);
                    
                    close(sock);
                    free(tab);
                    free(finfo);
                    return 0;
                  }else{
                    fprintf(stderr,"app_mess : Problem with getaddrinfo : struct sockaddrinfo finfo is NULL\n");
                  }
                }else{
                  fprintf(stderr,"app_mess : Problem with getaddrinfo %d\n",r);
                }
              }
            }else{
              fprintf(stderr,"app_mess ; Wrong application id %s\n",tab[2]);
            }
          }
        }else{
          fprintf(stderr,"app_mess ; Wrong message id %s\n",tab[1]);
        }
      }    
    }
  }
  return -1;
}

/*
 * Function managing WHOS udp message protocol
 */
int whos(uEntity* u, char* buff){
  if(strlen(buff)==13){
    //printf("Taille du message ok\n");
    int sock = socket(PF_INET,SOCK_DGRAM,0);
    struct addrinfo *finfo;
    struct addrinfo hints;
    
    bzero(&hints,sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    
    char* next_uport = malloc(sizeof(char)*100);
    sprintf(next_uport,"%d",(*(u->ent)).next_uport1);
		
    int r = getaddrinfo((*(u->ent)).next_ip1,next_uport,&hints,&finfo);
    if(r==0)
    {
      if(finfo!=NULL)
      {
        struct sockaddr *saddr = finfo->ai_addr;
        char** tab = split(buff,' ');
        //printf("Value of firdt idm %s\n",tab[1]);
        if(str_arrsize(tab)==2)
        {
          if (strcmp(tab[0],"WHOS")==0) {
            if (strlen(tab[1])==8) {
              //printf("Taille de idm ok  et valeur de isin %d\n",isin(u,tab[1]));
              if(isin(u,tab[1])==-1){
                //Case where message never has been managed
                char tampon[520];
                strcat(tampon,"MEMB ");
                char* idm = gen_idmess();
                strcat(tampon,idm);
                strcat(tampon," ");
                strcat(tampon,(*u).ent->id);
                strcat(tampon," ");
                strcat(tampon,ip_addZero((*u).ent->my_ip));
                strcat(tampon," ");
                char* udp = intchar((*u).ent->my_uport,4);
                if(udp != NULL){
                  strcat(tampon,udp);
                  
                  //printf("Value of MEMB send %s\n\n",tampon);
                  //We send message WHOS
                  sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
                  
                  //We send the answer MEMB
                  sendto(sock,tampon,strlen(tampon),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
                  
                  //Case for the second ring (duplication)
                  if((*u).ent->cast_ip2!=NULL && (*u).ent->next_ip2!=NULL 
                  && (*u).ent->cast_port2!=0 && (*u).ent->next_uport2!=0)
                  {
                    sprintf(next_uport,"%d",(*(u->ent)).next_uport2);
                    
                    r = getaddrinfo((*(u->ent)).next_ip2,next_uport,&hints,&finfo);
                    if(r==0)
                    {
                      if(finfo!=NULL)
                      {
                        struct sockaddr *saddr = finfo->ai_addr;
                        //We send message WHOS
                        sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
                        
                        //We send the answer MEMB
                        sendto(sock,tampon,strlen(tampon),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
                        
                        //Store idm tab[1] on array rec
                        add_umess(u,0,tab[1]);
                        add_umess(u,1,tab[1]);
                        //Store idm on array env
                        add_umess(u,0,idm);
                        
                        close(sock);
                        free(tab);
                        free(finfo);
                        return 0;
                      }else{
                        fprintf(stderr,"whos : Problem with get_addrinfo for the second ring struct sockaddrinfo *finfo is NULL \n");
                        return -1;
                      }
                    }else{
                      fprintf(stderr,"whos : Problem with get_addrinfo for the second ring %d\n",r);
                      return -1;
                    }
                  }
                  //Store idm tab[1] on array rec
                  add_umess(u,0,tab[1]);
                  add_umess(u,1,tab[1]);
                  //Store idm on array env
                  add_umess(u,0,idm);
                  
                  close(sock);
                  free(tab);
                  free(finfo);
                  return 0;
                }
              }
            }else{
              fprintf(stderr,"whos : Wrong idmess %s\n",tab[1]);
            }
          }    
        }      
      }else{
        fprintf(stderr,"whos : Problem with get_addrinfo struct sockaddrinfo *finfo is NULL \n");
      }
    }else{
      fprintf(stderr,"whos : Problem with get_addrinfo %d\n",r);
    }
  }else if (strlen(buff)==43)
  {
    int sock = socket(PF_INET,SOCK_DGRAM,0);
    struct addrinfo *finfo;
    struct addrinfo hints;
    
    bzero(&hints,sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    
    char* next_uport = malloc(sizeof(char)*100);
    sprintf(next_uport,"%d",(*(u->ent)).next_uport1);
		
    int r = getaddrinfo((*(u->ent)).next_ip1,next_uport,&hints,&finfo);
    if (r==0)
    {
      if (finfo!=NULL)
      {
        //struct sockaddr *saddr = finfo->ai_addr;
        char** tab = split(buff,' ');
        if(str_arrsize(tab)==5)
        {
          if(strcmp(tab[0],"MEMB")==0)
          {
            //Case where we received MEMB
            if(strlen(tab[1])==8){
              if(isin(u,tab[1])==-1){
                //Case where message has never been manage
                printf("whos_answer : %s\n",buff);
                
                struct sockaddr *saddr = finfo->ai_addr;
                //We send message WHOS
                sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
                
                //Case for the second ring (duplication)
                if((*u).ent->cast_ip2!=NULL && (*u).ent->next_ip2!=NULL 
                && (*u).ent->cast_port2!=0 && (*u).ent->next_uport2!=0)
                {
                  sprintf(next_uport,"%d",(*(u->ent)).next_uport2);
                  
                  r = getaddrinfo((*(u->ent)).next_ip2,next_uport,&hints,&finfo);
                  if (r==0)
                  {
                    if(finfo!=NULL){
                      struct sockaddr *saddr = finfo->ai_addr;
                      //We send message WHOS
                      sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
                      
                      //Store idm tab[1] on array rec
                      add_umess(u,0,tab[1]);
                      add_umess(u,1,tab[1]);
                      close(sock);
                      free(tab);
                      free(finfo);
                      return 0;
                    }else{
                      fprintf(stderr,"whos_answer : Problem with getaddrinfo for the second ring struct addrinfo finfo is NULL\n");
                      return -1;
                    }
                  }else{
                    fprintf(stderr,"whos_answer : Problem with getaddrinfo for the second ring %d\n",r);
                    return -1;
                  }                  
                }
                
                //Store idm tab[1] on array rec
                add_umess(u,0,tab[1]);
                add_umess(u,1,tab[1]);
                close(sock);
                free(tab);
                free(finfo);
                return 0;
              }
            }else{
              fprintf(stderr,"whos_answer : Wrong id message %s\n",tab[1]);
            }
          }
        }
      }else{
        fprintf(stderr,"whos_answer : Problem with getaddrinfo struct addrinfo finfo is NULL\n");
      }      
    }else{
      fprintf(stderr,"whos_answer : Problem with getaddrinfo %d\n",r);
    }
  }
  return -1;
}

/*
 * Function managing GBYE udp message protocol
 */
int gbye(uEntity* u, char* buff){
  if(strlen(buff)==55)
  {
    int sock = socket(PF_INET,SOCK_DGRAM,0);
    struct addrinfo *finfo;
    struct addrinfo hints;
    bzero(&hints,sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    
    char* next_uport = intchar((*u).ent->next_uport1,4);
    int r = getaddrinfo((*u).ent->next_ip1,next_uport,&hints,&finfo);
    if (r==0)
    {
      if (finfo!=NULL)
      {
        struct sockaddr *saddr = finfo->ai_addr;
        char** tab = split(buff,' ');
        if(str_arrsize(tab)==6)
        {
          if(strcmp(tab[0],"GBYE")==0)
          {
            if (strlen(tab[1])==8)
            {
              if(isin(u,tab[1])==-1){
                //Case where the message has never been seen
                char* ip = tab[2];
                int port = atoi(tab[3]);
                char* ipsucc = tab[4];
                int portsucc = atoi(tab[5]);
                if(strlen(ip)==15 && strlen(ipsucc)==15
                  && port>0 && port<=9999
                  && portsucc>0 && portsucc<=9999)
                {
                  if (check_ip(ip)!=-1 && check_ip(ipsucc)!=-1)
                  {
                    if (strcmp(ip,(*u).ent->next_ip1)==0 
                    && (*u).ent->next_uport1==port)
                    {
                      //Case previous entity
                      strcpy((*u).ent->next_ip1,ipsucc);
                      (*u).ent->next_uport1=portsucc;
                      char* tampon = malloc(sizeof(char)*120);
                      strcat(tampon,"EYBG ");
                      char* idm = gen_idmess();
                      strcat(tampon,idm);
                      sendto(sock,tampon,strlen(tampon),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
                      free(tab);
                      free(tampon);
                      close(sock);
                      return 0;
                    }else if((*u).ent->cast_ip2!=NULL && (*u).ent->next_ip2!=NULL 
                    && (*u).ent->cast_port2!=0 && (*u).ent->next_uport2!=0){
                      //Case for the second ring (duplication)
                      
                      if (strcmp(ip,(*u).ent->next_ip2)==0 
                          && (*u).ent->next_uport2==port)
                      {
                        next_uport = intchar((*u).ent->next_uport2,4);
                        r = getaddrinfo((*u).ent->next_ip2,next_uport,&hints,&finfo);
                        if (r==0)
                        {
                          if (finfo!=NULL)
                          {
                            saddr = finfo->ai_addr;
                            //Case previous entity
                            strcpy((*u).ent->next_ip2,ipsucc);
                            (*u).ent->next_uport2=portsucc;
                            char* tampon = malloc(sizeof(char)*120);
                            strcat(tampon,"EYBG ");
                            char* idm = gen_idmess();
                            strcat(tampon,idm);
                            sendto(sock,tampon,strlen(tampon),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
                            free(tab);
                            free(tampon);
                            close(sock);
                            return 0;
                          }else{
                            fprintf(stderr,"gbye : Problem with getaddrinfo for the second ring struct sockaddrinfo *finfo is NULL\n");
                            return -1;
                          }
                        }else{
                          fprintf(stderr,"gbye : Problem with getaddrinfo for the second ring %d\n",r);    
                          return -1;
                        }
                      }
                
                    }else{
                      //Case where entity is not concern by the message
                      
                      sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
                      
                      //Case for the second ring (duplication)
                      if((*u).ent->cast_ip2!=NULL && (*u).ent->next_ip2!=NULL 
                        && (*u).ent->cast_port2!=0 && (*u).ent->next_uport2!=0){
                      
                        next_uport = intchar((*u).ent->next_uport2,4);
                        r = getaddrinfo((*u).ent->next_ip2,next_uport,&hints,&finfo);
                        if (r==0)
                        {
                          if (finfo!=NULL)
                          {
                            saddr = finfo->ai_addr;
                            sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
                            free(tab);
                            free(finfo);
                            close(sock);
                            return 0;
                          }else{
                            fprintf(stderr,"gbye : Problem with getaddrinfo for the second ring struct sockaddrinfo *finfo is NULL\n");
                            return -1;
                          }
                        }else{
                          fprintf(stderr,"gbye : Problem with getaddrinfo for the second ring %d\n",r);    
                          return -1;
                        }
                      }
                      free(tab);
                      free(finfo);
                      close(sock);
                      return 0;
                    }
                  }else{
                    fprintf(stderr,"gbye : Unvalid ip %s or ipsucc %s\n",ip,ipsucc);
                  }
                }else{
                  fprintf(stderr,"gbye : Wrong form of the message received, containing invalid arguments %s\n",buff);
                }
              }
            }
          }
        }
      }else{
        fprintf(stderr,"gbye : Problem with getaddrinfo struct sockaddrinfo *finfo is NULL\n");
      }
    }else{
      fprintf(stderr,"gbye : Problem with getaddrinfo %d\n",r);    
    }
  }else if(strlen(buff)==13){
    char** tab = split(buff,' ');
    if(strcmp("EYBG",tab[0])==0)
    {
      if(isin(u,buff)==-1)
      {
        printf("gbye : Message received %s\n\tNew configuration for next on ring : ip %s\tudp port %d\n",buff,(*u).ent->next_ip1,(*u).ent->next_uport1);
        if((*u).ent->cast_ip2!=NULL && (*u).ent->next_ip2!=NULL 
                        && (*u).ent->cast_port2!=0 && (*u).ent->next_uport2!=0){
          printf("gbye : Message received %s\n\tNew configuration for next : ip %s\tudp port %d\n",buff,(*u).ent->next_ip2,(*u).ent->next_uport2);
        }
        return 0;
      }
    }
  }
  return -1;
}

/*
 * Function managing TEST udp message protocol
 */
int testring(uEntity* u, char* buff){
  if (strlen(buff)==34)
  {
    char** tab = split(buff, ' ');
    if(str_arrsize(tab)==4)
    {
      if(strcmp(tab[0],"TEST")==0)
      {
        if(strlen(tab[1])==8)
        {
          if(isin(u,tab[1])==-1)
          {
            char* ipdiff = tab[2];
            int portdiff = atoi(tab[3]);
            if(strlen(ipdiff)==15 && portdiff>0 && portdiff<=9999)
            {
              if(check_ip(ipdiff)!=-1)
              {
                if(strcmp(ipdiff,(*u).ent->cast_ip1)==0 && portdiff==(*u).ent->cast_port1)
                {
                  int sock = socket(PF_INET,SOCK_DGRAM,0);
                  struct addrinfo *finfo;
                  struct addrinfo hints;
                  bzero(&hints,sizeof(struct addrinfo));
                  hints.ai_family = AF_INET;
                  hints.ai_socktype = SOCK_DGRAM;
                  
                  char* next_uport = intchar((*u).ent->next_uport1,4);
                  int r = getaddrinfo((*u).ent->next_ip1,next_uport,&hints,&finfo);
                  if (r==0)
                  {
                    if (finfo!=NULL){
                      struct sockaddr *saddr = finfo->ai_addr;
                      sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
                      add_umess(u,0,tab[1]);
                      add_umess(u,1,tab[1]);
                      free(tab);
                      free(finfo);
                      close(sock);
                      return 0;
                    }else{
                      fprintf(stderr,"testring : Problem with getaddrinfo struct addrinfo finfo is NULL");
                      return -1;
                    }
                  }else{
                    fprintf(stderr,"testring : Problem with getaddrinfo %d\n",r);
                    return -1;
                  }
                }else if((*u).ent->cast_ip2!=NULL && (*u).ent->next_ip2!=NULL 
                        && (*u).ent->cast_port2!=0 && (*u).ent->next_uport2!=0)
                {
                  if(strcmp(ipdiff,(*u).ent->cast_ip2)==0 && portdiff==(*u).ent->cast_port2)
                  {
                    int sock = socket(PF_INET,SOCK_DGRAM,0);
                    struct addrinfo *finfo;
                    struct addrinfo hints;
                    bzero(&hints,sizeof(struct addrinfo));
                    hints.ai_family = AF_INET;
                    hints.ai_socktype = SOCK_DGRAM;
                    
                    char* next_uport = intchar((*u).ent->next_uport2,4);
                    int r = getaddrinfo((*u).ent->next_ip2,next_uport,&hints,&finfo);
                    if (r==0)
                    {
                      if (finfo!=NULL){
                        struct sockaddr *saddr = finfo->ai_addr;
                        sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
                        add_umess(u,0,tab[1]);
                        add_umess(u,1,tab[1]);
                        free(tab);
                        free(finfo);
                        close(sock);
                        return 0;
                      }else{
                        fprintf(stderr,"testring : Problem with getaddrinfo for the second ring struct addrinfo finfo is NULL");
                        return -1;
                      }
                    }else{
                      fprintf(stderr,"testring : Problem with getaddrinfo for the second ring %d\n",r);
                      return -1;
                    }
                  }else{
                    return -1;
                  }
                } 
              }else{
                fprintf(stderr,"testring : Wrong message form, problem with ip_diff %s\n",ipdiff);
              }
            }else{
              fprintf(stderr,"testring : Wrong message form, probleme with legnth of one the arguments\n");
            }
          }
        }
      }
    }
  }
  return -1;
}


/*Global fonction for udp message*/
void* rec_udp(void* uent){
  uEntity* u = (uEntity*)uent;
  int sock = socket(PF_INET,SOCK_DGRAM,0);
  struct sockaddr_in address_sock;
  address_sock.sin_family = AF_INET;
  if((*u).ent->my_uport<=9999 && (*u).ent->my_uport>0){
    address_sock.sin_port = htons((*u).ent->my_uport);
    //inet_aton((*u).ent->my_ip,&address_sock.sin_addr);
    address_sock.sin_addr.s_addr=htonl(INADDR_ANY);
    int r = bind(sock,(struct sockaddr*)&address_sock,sizeof(struct sockaddr_in));
    if (r==0) {
      char buff[512];
      while (1) {
        int rec = recv(sock,buff,512,0);
        if(rec>0){
          buff[rec]='\0';
          printf("\nrec_udp : Message received %s\n\n",buff);
          /*reste à coder */
          if(app_mess(u,buff)!=0)
          {
            if(whos(u,buff)!=0)
            {
              if(gbye(u,buff)!=0)
              {
                if(testring(u,buff)!=0)
                {
                  fprintf(stderr,"rec_udp : No protocol for manage the message or the message has already been treated %s\n",buff);
                }
              }
            }
          }
        }
      }
    }else {
      fprintf(stderr,"rec_udp bind: Problem with socket binding %d\n",r);
    }
  }else{
    fprintf(stderr,"rec_udp : Wrong udp port entity\n");
  }
  return NULL;
}




//////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////ENVOI////////////////////////////////////////////////////////////


int gen_appmess(uEntity* u, char* mess){
  if(mess!=NULL && strlen(mess)<=488 && strlen(mess)>0)
  {
    if((*u).id_app!=NULL && (*u).ent->next_ip1!=NULL &&
       (*u).ent->next_uport1>0 ){
      char* idm = gen_idmess();
      int sock = socket(PF_INET,SOCK_DGRAM,0);
      struct addrinfo *finfo;
      struct addrinfo hints;
      bzero(&hints,sizeof(struct addrinfo));
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_DGRAM;
      char* next_uport = intchar((*u).ent->next_uport1,4);
      int r = getaddrinfo((*u).ent->next_ip1,next_uport,&hints,&finfo);
      if (r==0) 
      {
        if (finfo!=NULL) 
        {
          struct sockaddr *saddr = finfo->ai_addr;
          char* buff = "APPL ";
          strcat(buff,idm);
          strcat(buff," ");
          strcat(buff,(*u).id_app);
          strcat(buff," ");
          strcat(buff,mess);
          sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
          
          if((*u).ent->cast_ip2!=NULL && (*u).ent->next_ip2!=NULL 
                      && (*u).ent->cast_port2!=0 && (*u).ent->next_uport2!=0){
            next_uport = intchar((*u).ent->next_uport2,4);
            r = getaddrinfo((*u).ent->next_ip2,next_uport,&hints,&finfo);
            if (r==0) 
            {
              if (finfo!=NULL) 
              {
                struct sockaddr *saddr = finfo->ai_addr;
                
                sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
                printf("gen_appmess : Message sent %s\n",buff);
                
                add_umess(u,0,idm);
                add_umess(u,1,idm);
                free(finfo);
                close(sock);
                return 0;
              }else{
                fprintf(stderr,"gen_appmess : Problem with getaddrinfo for the second ring struct addrinfo finfo is NULL\n");
                return -1;
              }
            }else{
              fprintf(stderr,"gen_appmess : Problem with getaddrinfo for the second ring %d\n",r);
              return -1;
            }
          }else{
            printf("gen_appmess : Message sent %s\n",buff);
          }
          add_umess(u,0,idm);
          add_umess(u,1,idm);
          free(finfo);
          close(sock);
          return 0;
        }else{
          fprintf(stderr,"gen_appmess : Problem with getaddrinfo struct addrinfo finfo is NULL\n");
        }
      }else{
        fprintf(stderr,"gen_appmess : Problem with getaddrinfo %d\n",r);
      }
    }
  }else{
    fprintf(stderr,"gen_appmess : Problem with the message of application %s\n",mess);
  }
  return -1;
}

int gen_whosmess(uEntity* u){
  if((*u).id_app!=NULL && (*u).ent->next_ip1!=NULL &&
       (*u).ent->next_uport1>0 ){
    char* idm = gen_idmess();
    int sock = socket(PF_INET,SOCK_DGRAM,0);
    struct addrinfo *finfo;
    struct addrinfo hints;
    bzero(&hints,sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    char* next_uport = intchar((*u).ent->next_uport1,4);
    int r = getaddrinfo((*u).ent->next_ip1,next_uport,&hints,&finfo);
    if (r==0) 
    {
      if (finfo!=NULL) 
      {
        struct sockaddr *saddr = finfo->ai_addr;
        char* buff = "WHOS ";
        strcat(buff,idm);
        sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
        
        if((*u).ent->cast_ip2!=NULL && (*u).ent->next_ip2!=NULL 
                    && (*u).ent->cast_port2!=0 && (*u).ent->next_uport2!=0){
          next_uport = intchar((*u).ent->next_uport2,4);
          r = getaddrinfo((*u).ent->next_ip2,next_uport,&hints,&finfo);
          if (r==0) 
          {
            if (finfo!=NULL) 
            {
              struct sockaddr *saddr = finfo->ai_addr;
              
              sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
              printf("gen_whosmess : Message sent %s\n",buff);
              
              add_umess(u,0,idm);
              add_umess(u,1,idm);
              free(finfo);
              close(sock);
              return 0;
            }else{
              fprintf(stderr,"gen_whosmess : Problem with getaddrinfo for the second ring struct addrinfo finfo is NULL\n");
              return -1;
            }
          }else{
            fprintf(stderr,"gen_whosmess : Problem with getaddrinfo for the second ring %d\n",r);
            return -1;
          }
        }else{
          printf("gen_whosmess : Message sent %s\n",buff);
        }
        add_umess(u,0,idm);
        add_umess(u,1,idm);
        free(finfo);
        close(sock);
        return 0;
      }else{
        fprintf(stderr,"gen_whosmess : Problem with getaddrinfo struct addrinfo finfo is NULL\n");
      }
    }else{
      fprintf(stderr,"gen_whosmess : Problem with getaddrinfo %d\n",r);
    }     
  }
  return -1;
}

int gen_gbyemess(uEntity* u, int ring){
  if(ring==1){
    if((*u).id_app!=NULL && (*u).ent->my_ip!=NULL && (*u).ent->next_ip1!=NULL &&
         (*u).ent->next_uport1>0 && (*u).ent->my_uport>0
         && (*u).ent->next_uport1<=9999 && (*u).ent->my_uport<=9999){
      char* idm = gen_idmess();
      int sock = socket(PF_INET,SOCK_DGRAM,0);
      struct addrinfo *finfo;
      struct addrinfo hints;
      bzero(&hints,sizeof(struct addrinfo));
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_DGRAM;
      char* next_uport = intchar((*u).ent->next_uport1,4);
      int r = getaddrinfo((*u).ent->next_ip1,next_uport,&hints,&finfo);
      if (r==0) 
      {
        if (finfo!=NULL) 
        {
          struct sockaddr *saddr = finfo->ai_addr;
          char* buff = "GBYE ";
          strcat(buff,idm);
          strcat(buff," ");
          strcat(buff, ip_addZero((*u).ent->my_ip));
          strcat(buff," ");
          strcat(buff,intchar((*u).ent->my_uport,4));
          strcat(buff," ");
          strcat(buff, ip_addZero((*u).ent->next_ip1));
          strcat(buff," ");
          strcat(buff,next_uport);
          
          sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
          printf("gen_gbyemess : Message sent %s\n",buff);
          
          add_umess(u,0,idm);
          add_umess(u,1,idm);
          free(finfo);
          close(sock);
          return 0;
        }else{
          fprintf(stderr,"gen_gbyemess : Problem with getaddrinfo struct addrinfo finfo is NULL\n");
        }
      }else{
        fprintf(stderr,"gen_gbyemess : Problem with getaddrinfo %d\n",r);
      }     
    }else{
      fprintf(stderr,"gen_gbyemess : Problem with arguments of entity\n");
    }
  }else if(ring==2){
    if((*u).id_app!=NULL && (*u).ent->my_ip!=NULL && (*u).ent->next_ip2!=NULL &&
         (*u).ent->next_uport2>0 && (*u).ent->my_uport>0
         && (*u).ent->next_uport2<=9999 && (*u).ent->my_uport<=9999){
      char* idm = gen_idmess();
      int sock = socket(PF_INET,SOCK_DGRAM,0);
      struct addrinfo *finfo;
      struct addrinfo hints;
      bzero(&hints,sizeof(struct addrinfo));
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_DGRAM;
      char* next_uport = intchar((*u).ent->next_uport2,4);
      int r = getaddrinfo((*u).ent->next_ip2,next_uport,&hints,&finfo);
      if (r==0) 
      {
        if (finfo!=NULL) 
        {
          struct sockaddr *saddr = finfo->ai_addr;
          char* buff = "GBYE ";
          strcat(buff,idm);
          strcat(buff," ");
          strcat(buff, ip_addZero((*u).ent->my_ip));
          strcat(buff," ");
          strcat(buff,intchar((*u).ent->my_uport,4));
          strcat(buff," ");
          strcat(buff, ip_addZero((*u).ent->next_ip2));
          strcat(buff," ");
          strcat(buff,next_uport);
          
          sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
          printf("gen_gbyemess : Message sent %s\n",buff);
          
          add_umess(u,0,idm);
          add_umess(u,1,idm);
          free(finfo);
          close(sock);
          return 0;
        }else{
          fprintf(stderr,"gen_gbyemess : Problem with getaddrinfo for the second ring struct addrinfo finfo is NULL\n");
        }
      }else{
        fprintf(stderr,"gen_gbyemess : Problem with getaddrinfo for the second ring %d\n",r);
      }     
    }else{
      fprintf(stderr,"gen_gbyemess : Problem with arguments of entity\n");
    }
  }
  return -1;
}

int gen_testmess(uEntity* u, int ring){
  if(ring==1){
    if((*u).ent->next_ip1!=NULL && (*u).ent->cast_ip1!=NULL
      && (*u).ent->next_uport1>0 && (*u).ent->cast_port1>0
      && (*u).ent->next_uport1<=9999 && (*u).ent->cast_port1<=9999)
    {
      char* idm = gen_idmess();
      int sock = socket(PF_INET,SOCK_DGRAM,0);
      struct addrinfo *finfo;
      struct addrinfo hints;
      bzero(&hints,sizeof(struct addrinfo));
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_DGRAM;
      char* next_uport = intchar((*u).ent->next_uport1,4);
      int r = getaddrinfo((*u).ent->next_ip1,next_uport,&hints,&finfo);
      if (r==0) 
      {
        if (finfo!=NULL) 
        {
          struct sockaddr *saddr = finfo->ai_addr;
          char* buff = "TEST ";
          strcat(buff,idm);
          strcat(buff," ");
          strcat(buff, ip_addZero((*u).ent->cast_ip1));
          strcat(buff," ");
          strcat(buff,intchar((*u).ent->cast_port1,4));
          
          sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
          printf("gen_testmess : Message sent %s\n",buff);
          
          add_umess(u,0,idm);
          add_umess(u,1,idm);
          free(finfo);
          close(sock);
          return 0;
        }else{
          fprintf(stderr,"gen_testmess : Problem with getaddrinfo struct addrinfo finfo is NULL\n");
        }
      }else{
        fprintf(stderr,"gen_testmess : Problem with getaddrinfo %d\n",r);
      }     
    }else{
      fprintf(stderr,"gen_testmess : Problem with arguments of entity\n");
    }
  }else if (ring == 2)
  {
    if((*u).ent->next_ip2!=NULL && (*u).ent->cast_ip2!=NULL
      && (*u).ent->next_uport2>0 && (*u).ent->cast_port2>0
      && (*u).ent->next_uport2<=9999 && (*u).ent->cast_port2<=9999)
    {
      char* idm = gen_idmess();
      int sock = socket(PF_INET,SOCK_DGRAM,0);
      struct addrinfo *finfo;
      struct addrinfo hints;
      bzero(&hints,sizeof(struct addrinfo));
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_DGRAM;
      char* next_uport = intchar((*u).ent->next_uport2,4);
      int r = getaddrinfo((*u).ent->next_ip2,next_uport,&hints,&finfo);
      if (r==0) 
      {
        if (finfo!=NULL) 
        {
          struct sockaddr *saddr = finfo->ai_addr;
          char* buff = "TEST ";
          strcat(buff,idm);
          strcat(buff," ");
          strcat(buff, ip_addZero((*u).ent->cast_ip2));
          strcat(buff," ");
          strcat(buff,intchar((*u).ent->cast_port2,4));
          
          sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
          printf("gen_testmess : Message sent %s\n",buff);
          
          add_umess(u,0,idm);
          add_umess(u,1,idm);
          free(finfo);
          close(sock);
          return 0;
        }else{
          fprintf(stderr,"gen_testmess : Problem with getaddrinfo for the second ring struct addrinfo finfo is NULL\n");
        }
      }else{
        fprintf(stderr,"gen_testmess : Problem with getaddrinfo for the second ring %d\n",r);
      }     
    }else{
      fprintf(stderr,"gen_testmess : Problem with arguments of entity\n");
    }
  }
  return -1;
}

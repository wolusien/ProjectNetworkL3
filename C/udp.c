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

int init_uEntity(uEntity* u){
  entity* e = malloc(sizeof(entity));
  int r = init_entity(e);
  if(r==0){
    (*u).ent = e;
    (*u).rec_size = (*u).env_size = 100;
    (*u).rec_pos = (*u).env_pos = 0;
    (*u).rec = malloc(sizeof(char*)*(*u).rec_size);
    (*u).env = malloc(sizeof(char*)*(*u).env_size);
    (*u).count_time = 0;
    return 0; 
  }
  return -1;
} 


//////////////////////////////////////////////////////////////////////
///////////////////RECEPTION//////////////////////////////////////////

//Function managing application udp message protocol
int app_mess(uEntity* u, char* buff){
  if(strlen(buff)<=512){
    int sock = socket(PF_INET,SOCK_DGRAM,0);
    
    struct sockaddr_in adress_sock;
    adress_sock.sin_family = AF_INET;
    adress_sock.sin_port = htons((*u).ent->next_uport1);
    int inet = inet_aton((*u).ent->next_ip1,&(adress_sock.sin_addr));
    if(inet == 1)
    {
      //char* next_uport = intchar((*u).ent->next_uport1,4);
      char** tab = split(buff,' ');
      if(str_arrsize(tab)>3){
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
                  
                  sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&adress_sock,(socklen_t)sizeof(struct sockaddr_in));
                  printf("app_mess : Message send %s\n",buff);
                  
                  //Case for the second ring (duplication)
                  if((*u).ent->cast_ip2!=NULL && (*u).ent->next_ip2!=NULL && (*u).ent->cast_port2!=0 && (*u).ent->next_uport2!=0)
                  { 
                    adress_sock.sin_port = htons((*u).ent->next_uport2);
                    inet = inet_aton((*u).ent->next_ip2,&adress_sock.sin_addr);
                    //char* next_uport = intchar((*u).ent->next_uport2,4);
                    
                    if(inet == 1)
                    {
                      sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&(adress_sock),(socklen_t)sizeof(struct sockaddr_in));
                      printf("app_mess :  Message send on the second ring %s\n",buff);
                      
                      add_umess(u,0,tab[0]);
                      add_umess(u,0,tab[1]);
                
                      close(sock);
                      free(tab);
                      printf("J'ai fini");
                      return 0;
                    
                    }else{
                      perror("app_mess : ");
                    }
                  }
                  add_umess(u,0,tab[0]);
                  add_umess(u,0,tab[1]);
                  
                  close(sock);
                  free(tab);
                  return 0;                    
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
    }else{
      perror("app_mess : ");
    }
  }
  return -1;
}


//Function managing WHOS udp message protocol
int whos(uEntity* u, char* buff){
  if(strlen(buff)==13){
    //printf("Taille du message ok\n");
    int sock = socket(PF_INET,SOCK_DGRAM,0);
    
    struct sockaddr_in adress_sock;
    adress_sock.sin_family = AF_INET;
    adress_sock.sin_port = htons((*u).ent->next_uport1);
    int inet = inet_aton((*u).ent->next_ip1,&adress_sock.sin_addr);
    
    if(inet == 1)
    {
      //char* next_uport = intchar((*u).ent->next_uport1,4);
      char** tab = split(buff,' ');
      //printf("Value of firdt idm %s\n",tab[1]);
      if(str_arrsize(tab)==2)
      {
        if (strcmp(tab[0],"WHOS")==0) {
          if (strlen(tab[1])==8) {
            //printf("Taille de idm ok  et valeur de isin %d\n",isin(u,tab[1]));
            if(isin(u,tab[1])==-1){
              //Case where message never has been managed
              char tampon[520] = "MEMB ";
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
                sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&adress_sock,(socklen_t)sizeof(struct sockaddr_in));
                printf("whos : Message sent %s\n",buff);
                
                //We send the answer MEMB
                sendto(sock,tampon,strlen(tampon),0,(struct sockaddr*)&adress_sock,(socklen_t)sizeof(struct sockaddr_in));
                printf("whos : Message sent %s\n",tampon);
                
                //Case for the second ring (duplication)
                if((*u).ent->cast_ip2!=NULL && (*u).ent->next_ip2!=NULL 
                && (*u).ent->cast_port2!=0 && (*u).ent->next_uport2!=0)
                {
                  //char* next_uport=intchar((*u).ent->next_uport2,4);
                  adress_sock.sin_port = htons((*u).ent->next_uport2);
                  int inet = inet_aton((*u).ent->next_ip2,&adress_sock.sin_addr);
                  
                  if(inet == 1)
                  {
                    //We send message WHOS
                    sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&adress_sock,(socklen_t)sizeof(struct sockaddr_in));
                    printf("whos : Message sent on the second ring %s\n",buff);
                    
                    //We send the answer MEMB
                    sendto(sock,tampon,strlen(tampon),0,(struct sockaddr*)&adress_sock,(socklen_t)sizeof(struct sockaddr_in));
                    printf("whos : Message sent on the second ring %s\n",tampon);
                    
                    //Store idm tab[1] on array rec
                    add_umess(u,0,tab[1]);
                    add_umess(u,1,tab[1]);
                    //Store idm on array env
                    add_umess(u,0,idm);
                    
                    close(sock);
                    free(tab);
                    return 0;
                  }else{
                    perror("whos : ");
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
                return 0;
              }
            }
          }else{
            fprintf(stderr,"whos : Wrong idmess %s\n",tab[1]);
          }
        }    
      }
    }else{
      perror("whos : ");
    }
  }else if (strlen(buff)==43)
  {
    int sock = socket(PF_INET,SOCK_DGRAM,0);

    struct sockaddr_in adress_sock;
    adress_sock.sin_family = AF_INET;
    adress_sock.sin_port = htons((*u).ent->next_uport1);
    int inet = inet_aton((*u).ent->next_ip1,&adress_sock.sin_addr);
    
    if(inet == 1)
    {
      //char* next_uport = intchar((*u).ent->next_uport1,4);
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
              
              //We send message WHOS
              sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&(adress_sock),(socklen_t)sizeof(struct sockaddr_in));
              
              //Case for the second ring (duplication)
              if((*u).ent->cast_ip2!=NULL && (*u).ent->next_ip2!=NULL 
              && (*u).ent->cast_port2!=0 && (*u).ent->next_uport2!=0)
              {
                //char *next_uport=intchar((*u).ent->next_uport2,4);
                
                adress_sock.sin_port = htons((*u).ent->next_uport2);
                inet = inet_aton((*u).ent->next_ip2,&adress_sock.sin_addr);
      
                if(inet == 1)
                {
                  //We send message WHOS
                  sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&(adress_sock),(socklen_t)sizeof(struct sockaddr_in));
                  printf("whos_anwser : Message sent on the second ring %s\n",buff);
                  
                  //Store idm tab[1] on array rec
                  add_umess(u,0,tab[1]);
                  add_umess(u,1,tab[1]);
                  close(sock);
                  free(tab);
                  return 0;
                }else{
                  perror("whos : ");
                  return -1;
                }
              }
              
              //Store idm tab[1] on array rec
              add_umess(u,0,tab[1]);
              add_umess(u,1,tab[1]);
              close(sock);
              free(tab);
              return 0;
            }
          }else{
            fprintf(stderr,"whos_answer : Wrong id message %s\n",tab[1]);
          }
        }
      }
    }else{
      perror("whos : ");
      return -1;
    }
  }
  return -1;
}


//Function managing GBYE udp message protocol
int gbye(uEntity* u, char* buff){
  if(strlen(buff)==55)
  {
    int sock = socket(PF_INET,SOCK_DGRAM,0);
    
    struct sockaddr_in adress_sock;
    adress_sock.sin_family = AF_INET;
    adress_sock.sin_port = htons((*u).ent->next_uport1);
    int inet = inet_aton((*u).ent->next_ip1,&adress_sock.sin_addr);
    
    if(inet == 1)
    {
      //char* next_uport = intchar((*u).ent->next_uport1,4);
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
                    char tampon[120] = "EYBG ";
                    char* idm = gen_idmess();
                    strcat(tampon,idm);
                    sendto(sock,tampon,strlen(tampon),0,(struct sockaddr*)&(adress_sock),(socklen_t)sizeof(struct sockaddr_in));
                    printf("gbye : Message sent %s\n",tampon);
                    free(tab);
                    close(sock);
                    return 0;
                  }else if((*u).ent->cast_ip2!=NULL && (*u).ent->next_ip2!=NULL 
                  && (*u).ent->cast_port2!=0 && (*u).ent->next_uport2!=0){
                    //Case for the second ring (duplication)
                    
                    if (strcmp(ip,(*u).ent->next_ip2)==0 
                        && (*u).ent->next_uport2==port)
                    {
                      adress_sock.sin_port = htons((*u).ent->next_uport2);
                      inet = inet_aton((*u).ent->next_ip2,&adress_sock.sin_addr);
                      
                      if(inet == 1)
                      {
                        //next_uport = intchar((*u).ent->next_uport2,4);
                        //Case previous entity
                        strcpy((*u).ent->next_ip2,ipsucc);
                        (*u).ent->next_uport2=portsucc;
                        char tampon[120] = "EYBG ";
                        char* idm = gen_idmess();
                        strcat(tampon,idm);
                        sendto(sock,tampon,strlen(tampon),0,(struct sockaddr*)&(adress_sock),(socklen_t)sizeof(struct sockaddr_in));
                        printf("gbye : Message sent on the second ring %s\n",tampon);
                        
                        free(tab);
                        close(sock);
                        return 0;
                      }else{
                        perror("gbye : On the second ring ");
                        return -1;
                      }
                    }
              
                  }else{
                    //Case where entity is not concern by the message
                    
                    sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&(adress_sock),(socklen_t)sizeof(struct sockaddr_in));
                    printf("gbye : Message sent %s\n",buff);
                    
                    //Case for the second ring (duplication)
                    if((*u).ent->cast_ip2!=NULL && (*u).ent->next_ip2!=NULL 
                      && (*u).ent->cast_port2!=0 && (*u).ent->next_uport2!=0){
                    
                      //char *next_uport = intchar((*u).ent->next_uport2,4);
                      
                      adress_sock.sin_port = htons((*u).ent->next_uport2);
                      inet = inet_aton((*u).ent->next_ip2,&adress_sock.sin_addr);
                      
                      if(inet == 1)
                      {
                        sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&(adress_sock),(socklen_t)sizeof(struct sockaddr_in));
                        printf("gbye : Message sent on the second ring %s\n",buff);
                        
                        free(tab);
                        close(sock);
                        return 0;
                        
                      }else{
                        perror("gbye : On the second ring ");
                        return -1;
                      }
                    }
                    free(tab);
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
      perror("gbye : ");
      return -1;
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



//Function managing TEST udp message protocol
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
                  struct sockaddr_in adress_sock;
                  adress_sock.sin_family = AF_INET;
                  adress_sock.sin_port = htons((*u).ent->next_uport1);
                  int inet = inet_aton((*u).ent->next_ip1,&adress_sock.sin_addr);
                  
                  if(inet == 1)
                  {               
                    sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&(adress_sock),(socklen_t)sizeof(struct sockaddr_in));
                    add_umess(u,0,tab[1]);
                    add_umess(u,1,tab[1]);
                    free(tab);
                    close(sock);
                    return 0;                  
                  }else{
                    perror("testring : ");
                  }
                }else if((*u).ent->cast_ip2!=NULL && (*u).ent->next_ip2!=NULL 
                        && (*u).ent->cast_port2!=0 && (*u).ent->next_uport2!=0)
                {
                  if(strcmp(ipdiff,(*u).ent->cast_ip2)==0 && portdiff==(*u).ent->cast_port2)
                  {
                    int sock = socket(PF_INET,SOCK_DGRAM,0);
                    struct sockaddr_in adress_sock;
                    adress_sock.sin_family = AF_INET;
                    adress_sock.sin_port = htons((*u).ent->next_uport2);
                    int inet = inet_aton((*u).ent->next_ip2,&adress_sock.sin_addr);
                    
                    if(inet == 1)
                    {   
                      sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&(adress_sock),(socklen_t)sizeof(struct sockaddr_in));
                      add_umess(u,0,tab[1]);
                      add_umess(u,1,tab[1]);
                      free(tab);
                      close(sock);
                      return 0;
                    }else{
                      perror("testring : ");
                    }
                  }else{
                    return -1;
                  }
                } 
              }else{
                fprintf(stderr,"testring : Wrong message form, problem with ip_diff %s\n",ipdiff);
              }
            }else{
              fprintf(stderr,"testring : Wrong message form, probleme with length of one the arguments\n");
            }
          }
        }
      }
    }
  }
  return -1;
}


//Global fonction for udp message
void* rec_udp(void* uent){
  printf("Je lance le serveur de reception udp\n");
  uEntity* u = (uEntity*)uent;
  int sock = socket(PF_INET,SOCK_DGRAM,0);
  struct sockaddr_in address_sock;
  address_sock.sin_family = AF_INET;
  if((*u).ent->my_uport<=9999 && (*u).ent->my_uport>0){
    //printf("Je traite l'addresse %s et le port udp %d\n",(*u).ent->my_ip,(*u).ent->my_uport);
    address_sock.sin_port = htons((*u).ent->my_uport);
    int inet = inet_aton((*u).ent->my_ip,&(address_sock.sin_addr));
    if(inet == 1){
      int r = bind(sock,(struct sockaddr*)&address_sock,sizeof(struct sockaddr_in));
      if (r==0) {
        char* buff = malloc(sizeof(char)*512);
        while (1) {
          int rec = recv(sock,buff,512,0);
          if(rec>0){
            buff[rec]='\0';
            printf("\nrec_udp : Message received %s\n\n",buff);
            /*
            double var = (*u).count_time;
            if((*u).count_time!=0){
              (*u).count_time = (double)(clock()-var)/CLOCKS_PER_SEC;
            }
            */
            //reste à coder 
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
        perror("rec_udp bind: Problem with socket binding");
      }
    }else{
      fprintf(stderr,"rec_udp : Problem for inet_aton with ip\n");
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
      struct sockaddr_in adress_sock;
      adress_sock.sin_family = AF_INET;
      adress_sock.sin_port = htons((*u).ent->next_uport1);
      int inet = inet_aton((*u).ent->next_ip1,&adress_sock.sin_addr);
      
      if(inet == 1)
      {
        char buff[512] = "APPL ";
        strcat(buff,idm);
        strcat(buff," ");
        strcat(buff,(*u).id_app);
        strcat(buff," ");
        strcat(buff,mess);
        sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&adress_sock,(socklen_t)sizeof(struct sockaddr_in));
        
        if((*u).ent->cast_ip2!=NULL && (*u).ent->next_ip2!=NULL 
                    && (*u).ent->cast_port2!=0 && (*u).ent->next_uport2!=0){
          //char* next_uport = intchar((*u).ent->next_uport2,4);
         
          adress_sock.sin_port = htons((*u).ent->next_uport2);
          inet = inet_aton((*u).ent->next_ip2,&adress_sock.sin_addr);
          
          if(inet == 1)
          {
            sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&adress_sock,(socklen_t)sizeof(struct sockaddr_in));
            printf("gen_appmess : Message sent %s\n",buff);
            
            add_umess(u,0,idm);
            add_umess(u,1,idm);
            close(sock);
            return 0;          
          }else{
              perror("gen_appmess : ");
          }
        }else{
          printf("gen_appmess : Message sent %s\n",buff);
        }
        add_umess(u,0,idm);
        add_umess(u,1,idm);
        close(sock);
        return 0;
      }else{
        perror("gen_appmess : ");
      }
    }
  }else{
    fprintf(stderr,"gen_appmess : Problem with the message of application %s\n",mess);
  }
  return -1;
}


int gen_whosmess(uEntity* u){
  if((*u).ent->next_ip1!=NULL &&
       (*u).ent->next_uport1>0 ){
    char* idm = gen_idmess();
    int sock = socket(PF_INET,SOCK_DGRAM,0);
    
    struct sockaddr_in adress_sock;
    adress_sock.sin_family = AF_INET;
    adress_sock.sin_port = htons((*u).ent->next_uport1);
    int inet = inet_aton((*u).ent->next_ip1,&adress_sock.sin_addr);
    
    if(inet == 1)
    {
      char buff[512] = "WHOS ";
      strcat(buff,idm);
      sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&adress_sock,(socklen_t)sizeof(struct sockaddr_in));
      
      if((*u).ent->cast_ip2!=NULL && (*u).ent->next_ip2!=NULL 
                  && (*u).ent->cast_port2!=0 && (*u).ent->next_uport2!=0){
        //char* next_uport = intchar((*u).ent->next_uport2,4);
        
        adress_sock.sin_port = htons((*u).ent->next_uport2);
        inet = inet_aton((*u).ent->next_ip2,&adress_sock.sin_addr);
        
        if(inet == 1)
        {
          sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&adress_sock,(socklen_t)sizeof(struct sockaddr_in));
          printf("gen_whosmess : Message sent %s\n",buff);
          
          add_umess(u,0,idm);
          add_umess(u,1,idm);
          close(sock);
          return 0;
        }else{
          perror("gen_whosmess : ");
        }
      }else{
        printf("gen_whosmess : Message sent %s\n",buff);
      }
      add_umess(u,0,idm);
      add_umess(u,1,idm);
      close(sock);
      return 0;
    }else{
      perror("gen_whosmess : ");
    }
  }
  return -1;
}


int gen_gbyemess(uEntity* u, int ring){
  if(ring==1){
    if((*u).ent->my_ip!=NULL && (*u).ent->next_ip1!=NULL &&
         (*u).ent->next_uport1>0 && (*u).ent->my_uport>0
         && (*u).ent->next_uport1<=9999 && (*u).ent->my_uport<=9999){
      char* idm = gen_idmess();
      int sock = socket(PF_INET,SOCK_DGRAM,0);
      
      struct sockaddr_in adress_sock;
      adress_sock.sin_family = AF_INET;
      adress_sock.sin_port = htons((*u).ent->next_uport1);
      int inet = inet_aton((*u).ent->next_ip1,&adress_sock.sin_addr);
      
      if(inet == 1)
      {
        char* next_uport = intchar((*u).ent->next_uport1,4);
        char buff[512] = "GBYE ";
        strcat(buff,idm);
        strcat(buff," ");
        strcat(buff, ip_addZero((*u).ent->my_ip));
        strcat(buff," ");
        strcat(buff,intchar((*u).ent->my_uport,4));
        strcat(buff," ");
        strcat(buff, ip_addZero((*u).ent->next_ip1));
        strcat(buff," ");
        strcat(buff,next_uport);
        
        sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&adress_sock,(socklen_t)sizeof(struct sockaddr_in));
        printf("gen_gbyemess : Message sent %s\n",buff);
        
        add_umess(u,0,idm);
        add_umess(u,1,idm);
        close(sock);
        return 0;
      }else{
          perror("gen_gbyemess : ");
      }
    }else{
      fprintf(stderr,"gen_gbyemess : Problem with arguments of entity\n");
    }
  }else if(ring==2){
    if((*u).ent->my_ip!=NULL && (*u).ent->next_ip2!=NULL &&
         (*u).ent->next_uport2>0 && (*u).ent->my_uport>0
         && (*u).ent->next_uport2<=9999 && (*u).ent->my_uport<=9999){
      char* idm = gen_idmess();
      
      int sock = socket(PF_INET,SOCK_DGRAM,0);
      
      struct sockaddr_in adress_sock;
      adress_sock.sin_family = AF_INET;
      adress_sock.sin_port = htons((*u).ent->next_uport2);
      int inet = inet_aton((*u).ent->next_ip2,&adress_sock.sin_addr);
      
      if(inet == 1)
      {
        char* next_uport = intchar((*u).ent->next_uport2,4);
        char buff[512] = "GBYE ";
        strcat(buff,idm);
        strcat(buff," ");
        strcat(buff, ip_addZero((*u).ent->my_ip));
        strcat(buff," ");
        strcat(buff,intchar((*u).ent->my_uport,4));
        strcat(buff," ");
        strcat(buff, ip_addZero((*u).ent->next_ip2));
        strcat(buff," ");
        strcat(buff,next_uport);
        
        sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&adress_sock,(socklen_t)sizeof(struct sockaddr_in));
        printf("gen_gbyemess : Message sent %s\n",buff);
        
        add_umess(u,0,idm);
        add_umess(u,1,idm);
        close(sock);
        return 0;
      }else{
        perror("gen_gbyemess : ");
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
      
      struct sockaddr_in adress_sock;
      adress_sock.sin_family = AF_INET;
      adress_sock.sin_port = htons((*u).ent->next_uport1);
      int inet = inet_aton((*u).ent->next_ip1,&adress_sock.sin_addr);
      
      if(inet == 1)
      {
        char buff[512] = "TEST ";
        strcat(buff,idm);
        strcat(buff," ");
        strcat(buff, ip_addZero((*u).ent->cast_ip1));
        strcat(buff," ");
        strcat(buff,intchar((*u).ent->cast_port1,4));
        
        sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&adress_sock,(socklen_t)sizeof(struct sockaddr_in));
        printf("gen_testmess : Message sent %s\n",buff);
        
        (*u).count_time = (double) clock()/CLOCKS_PER_SEC;
                  
        add_umess(u,0,idm);
        //add_umess(u,1,idm);
        close(sock);
        return 0;
      }else{
        perror("gen_testmess :  ");
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
      
      struct sockaddr_in adress_sock;
      adress_sock.sin_family = AF_INET;
      adress_sock.sin_port = htons((*u).ent->next_uport2);
      int inet = inet_aton((*u).ent->next_ip2,&adress_sock.sin_addr);
      
      if(inet == 1)
      {
        char buff[512] = "TEST ";
        strcat(buff,idm);
        strcat(buff," ");
        strcat(buff, ip_addZero((*u).ent->cast_ip2));
        strcat(buff," ");
        strcat(buff,intchar((*u).ent->cast_port2,4));
        
        sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&adress_sock,(socklen_t)sizeof(struct sockaddr_in));
        printf("gen_testmess : Message sent %s\n",buff);
        
        add_umess(u,0,idm);
        add_umess(u,1,idm);
        close(sock);
        return 0;
      }else{
        perror("gen_testmess : ");
      }
    }else{
      fprintf(stderr,"gen_testmess : Problem with arguments of entity\n");
    }
  }
  return -1;
}



void* envoi_udp(void* e){
  uEntity* u = (uEntity*) e; 
  char** tab;
  int taille;
  int ring;
     
  while(1){
    char *buff=malloc(sizeof(char)*512);
    read(STDIN_FILENO, buff, 512);
    
    printf("envoi_udp : Message recu %s",buff);
    tab = split(buff,' ');
    taille=str_arrsize(tab);
    if(taille==2){
      if(strcmp(tab[0],"APP")) {gen_appmess( u, tab[1]);}
      ring= atoi(tab[1]);
      if(ring==1||ring==2){
        if(strcmp(tab[0],"GBYE")) gen_gbyemess( u,ring);
        if(strcmp(tab[0],"TEST")) gen_testmess( u,ring);
      }
    }
    if(taille==1){
      
      if(strcmp(tab[0],"WHOS")){  
        //printf("I will use whose\n");
        gen_whosmess( u);
      }
    }
  }
}


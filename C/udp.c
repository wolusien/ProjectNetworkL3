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

int init_uEntity(uEntity* u, entity* e){
  if((*u).ent!=NULL){
    (*u).ent = e;
    return 0; 
  }
  return -1;
} 

/*Manage application message*/
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
    if(str_arrsize(tab)==4){
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
                return 0;
              }else{
                //Case where we transmit the message to the next entity
                if(r==0){
                  if(finfo!=NULL){
                    struct sockaddr *saddr = finfo->ai_addr;
                    sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
                    add_umess(u,0,tab[0]);
                    add_umess(u,0,tab[1]);
                    close(sock);
                    free(tab);
                    free(finfo);
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
        }else{
          fprintf(stderr,"app_mess ; Wrong message id %s\n",tab[1]);
        }
      }    
    }
  }
  return -1;
}

/*Manage WHOS message*/
int whos(uEntity* u, char* buff){
  if(strlen(buff)==13){
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
        if(str_arrsize(tab)==2)
        {
          if (strcmp(tab[0],"WHOS")==0) {
            if (strlen(tab[1])==8) {
              if(isin(u,tab[1])==-1){
                //Case where message never has been managed
                char* tampon = malloc(sizeof(char)*120);
                strcat(tampon,"MEMB ");
                char* idm = gen_idmess();
                strcat(tampon,idm);
                strcat(tampon," ");
                strcat(tampon,(*u).ent->id);
                strcat(tampon," ");
                strcat(tampon,(*u).ent->my_ip);
                strcat(tampon," ");
                char* udp = intchar((*u).ent->my_uport,4);
                if(udp != NULL){
                  //We send message WHOS
                  sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
                  //Store idm tab[1] on array rec
                  add_umess(u,0,tab[1]);
                  add_umess(u,1,tab[1]);
                  strcat(tampon,udp);
                  //We send the answer MEMB
                  sendto(sock,tampon,strlen(tampon),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
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
                //Code à écrire
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
      }      
    }
  }
  return -1;
}

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
                if(strlen(ip)==8 && strlen(ipsucc)==8
                  && port>0 && port<=9999
                  && portsucc>0 && portsucc<=9999)
                {
                  if (check_ip(ip)==0 && check_ip(ipsucc)==0)
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
                    }else{
                      //Case where entity is not concern by the message
                      sendto(sock,buff,strlen(buff),0,saddr,(socklen_t)sizeof(struct sockaddr_in));
                      free(tab);
                      free(saddr);
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
  }
  return -1;
}

/*Global fonction for udp message*/
void* rec_udp(void* uent){
  uEntity* u = (uEntity*)uent;
  int sock = socket(PF_INET,SOCK_DGRAM,0);
  struct sockaddr_in address_sock;
  address_sock.sin_family = AF_INET;
  if((*u).ent->my_uport>9999 && (*u).ent->my_uport<=65535){
    address_sock.sin_port = htons((*u).ent->my_uport);
    inet_aton((*u).ent->my_ip,&address_sock.sin_addr);
    
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
    }
  }else{
    fprintf(stderr,"rec_udp : Wrong udp port entity\n");
  }
  return NULL;
}

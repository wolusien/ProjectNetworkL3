#include "udp.h"

/* Function that generate id of UDP messages */
char* gen_idmess(){
  time_t t;
  char* tmp = malloc(sizeof(char)*7);
  struct timespec start;
  int r = clock_gettime(CLOCK_REALTIME,&start);
  if(r==0){
    char* idm = malloc(sizeof(char)*8);
    char* tab = "azertyuiopqsdfghjklmwxcvbn";
    srand((unsigned) time(&t));
    idm[0] = tab[rand()%26];
    int i = (int)start.tv_nsec;
    sprintf(tmp,"%d",i);
    strcat(idm,tmp);
    return idm;
  }
  return NULL;
}

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
  if(strlen(buff)==512){
    char** tab = split(buff,' ');
    if(str_arrsize(tab)==4){
      if (strcmp(tab[0],"APPL")==0) {
        if (strlen(tab[2])==8) {
          if(strcmp((*u).(*e).id_app,tab[2])==0){
            //Case where (*u).(*e) support the application
            
          }else{
            //Case where we transmit the message to the next entity
            
          }
        }
      }    
    }
  }
  return -1;
}

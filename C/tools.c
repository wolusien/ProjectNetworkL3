#include "tools.h"
#include "entity.h"



char * concat(char *rep,char c){
  int len;
  len=strlen(rep);
  char *newstr=malloc(len+sizeof(char));
  sprintf(newstr, "%s%c", rep, c);
  // printf("new chaine %s",newstr);
  return newstr;
}

int count_space(char *rep,char delim){
  int i;
  int space;
  space=0;

  // printf("rentre");
  for(i=0;i<strlen(rep);i++){
    //  printf("lettre %c",rep[i]);
    if( rep[i]==delim)space++;
  }
  //  printf("nb espace %d",space);
  return space;
}

/*
  Function taking char* str and a char delim separator and split str in terms of separator
*/
char ** split(char *rep, char delim){
  //printf("rentre2");
  int i;
  int taille;
  int index;
  index=0;
  taille=count_space(rep,delim)+2;
  char ** split=malloc(taille*sizeof(char *));
  split[0]="";
  for(i=0;i<strlen(rep);i++){
    if( rep[i]==delim ){
      //printf("%s \n",split[index]);
      index++;
      split[index]="";
      //printf("%d \n",index);
    }
    else split[index]=concat(split[index],rep[i]);
  }
  split[taille-1]=NULL;
  //  printf("%s\n",split[2]);
  return split;
}

/*
  Return length of array of char*
*/
int str_arrsize(char** tab){
  int count = 0;
  while(tab[count]!=NULL){
    count++;
  }
  return count;
}


/*
  Function taking ip address of form "127.0.0.1" and change it to "127.000.000.001"
*/
char* ip_addZero(char* ip){
  int i;
  if(strlen(ip)>0 && strlen(ip)<15){
    char** tab = split(ip,'.');
    int size = str_arrsize(tab);
    char* ip_final = malloc(sizeof(char)*15);
    bzero(ip_final,sizeof(char)*15);
    for (i = 0; i < size; i++)
    {
      if(strlen(tab[i])==2){
        strcat(ip_final,"0");
      }
      else if(strlen(tab[i])==1){
        strcat(ip_final,"00");
      }
      strcat(ip_final,tab[i]);
      if(i!=size-1){
        strcat(ip_final,".");
      }
    }
    //printf("Value of ip addzero %s\n",ip_final);
    return ip_final;
  }else if(strlen(ip)==15){
    return ip;
  }
  return NULL;
}


char* ip_removeZero(char* ip){
  int i;
  if(ip!=NULL){
    if(strlen(ip)==15){
      char** tab = split(ip,'.');
      if(str_arrsize(tab)==4){
        char* ip_f = tab[0];
        int size = str_arrsize(tab);
        for (i = 1; i < size; i++)
        {
          strcat(ip_f,".");
          int r = atoi(tab[i]);
          char* t = malloc(sizeof(char)*3);
          sprintf(t,"%d",r);
          strcat(ip_f,t);
        }
        return ip_f;
      }
    }else{
      return ip;
    }
  }
  return NULL;
}

/*
  Get ip adress knowing the host
*/
char* get_ip(){
  struct ifaddrs *myaddrs, *ifa;
  struct sockaddr_in *s4;
  int status;
  char *ip=(char *)malloc(64*sizeof(char));
  status = getifaddrs(&myaddrs);
  if (status != 0){
    fprintf(stderr,"get_ip : Probleme de recuperation d'adresse IP");
    return NULL;
  }
  for (ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next){
    if (ifa->ifa_addr == NULL) continue;
    if ((ifa->ifa_flags & IFF_UP) == 0) continue;
    if ((ifa->ifa_flags & IFF_LOOPBACK) != 0) continue;
    if (ifa->ifa_addr->sa_family == AF_INET){
      s4 = (struct sockaddr_in *)(ifa->ifa_addr);
      if (inet_ntop(ifa->ifa_addr->sa_family, (void *)&(s4->sin_addr),
      ip, 64*sizeof(char)) != NULL){
        return ip;
      }
    }
  }
  freeifaddrs(myaddrs);
  return NULL;
}

/*
  Generate a unique code for id entity or application(normally)
*/
char* gen_code(){
  time_t t;
  char* tmp = malloc(sizeof(char)*7);
  struct timespec start;
  int r = clock_gettime(CLOCK_REALTIME,&start);
  if(r==0){
    char idm[8];
    char* tab = "azertyuiopqsdfghjklmwxcvbn";
    srand((unsigned) time(&t));
    idm[0] = tab[rand()%26];
    int i = (int)start.tv_nsec;
    sprintf(tmp,"%d",i);
    //printf("Value of time %s\n",tmp);
    for (i = 1; i <8 ; i++)
    {
      if(strlen(tmp)>7){
        idm[i]=tmp[i+1];
        //printf("Value of i %c\n",tmp[i+1]);
      }else{
        idm[i]=tmp[i];
      }
    }
    char* idf = malloc(sizeof(char)*8);
    strcpy(idf,idm);
    //printf("Value of id_entity %s\n",idf);
    return idf;
  }
  return NULL;
}

/*
  Allow to get a free tcp port on a host
*/
int free_tport(){
  int i;
  char* host = get_ip();
  int sock = socket(PF_INET,SOCK_STREAM,0);

  struct hostent* h;
  h=gethostbyname(host);
  if(h==NULL){
    printf("free_tport : Unknown host given\n");
    return -1;
  }
  char* host_addr;
  //Get list of addresses of the host
  struct in_addr **addresses = (struct in_addr**)h->h_addr_list;
  //We take the first address for testing the port
  while(*addresses != NULL){
    //inet_ntoa traduce struct in_addr to char*
    host_addr = inet_ntoa(**addresses);
    break;
  }

  if(host_addr!=NULL){
    struct sockaddr_in adress_sock;
    adress_sock.sin_family = AF_INET;
    int port = 1025;

    for (i = 0; i < 9999-1025; i++) {
      port += i;
      adress_sock.sin_port = htons(port);
      inet_aton(host_addr,&adress_sock.sin_addr);
      int con = connect(sock, (struct sockaddr *)&adress_sock,
                        sizeof(struct sockaddr_in));
      if(con != 0){
        close(sock);
        return port;
      }
    }
  }else{
    printf("free_tport : All the address are NULL for this host\n");
    return -1;
  }
  return -1;
}

/*
  Allow to get a free udp port on a host
*/
int free_uport(){
  int i;
  char* host = get_ip();
  int sock = socket(PF_INET,SOCK_DGRAM,0);
  
  struct sockaddr_in adress_sock;
  adress_sock.sin_family = AF_INET;
  
  if(host!=NULL){
    for (i = 1026; i <= 9999; i++) {
      adress_sock.sin_port = htons(i);
      inet_aton(host,&adress_sock.sin_addr);
      int b = bind(sock, (struct sockaddr *)&adress_sock,
                   sizeof(struct sockaddr_in));
      if(b == 0){
        close(sock);
        return i;
      }
    }
  }else{
    printf("free_uport : All the address are NULL for this host\n");
    return -1;
  }
  return -1;
}

/*
  Function that verify an ip_address
*/
int check_ip(char* ip){
  char* test = ip_removeZero(ip);
  struct sockaddr_in sa;
  int result = inet_pton(AF_INET, test, &(sa.sin_addr));
  if(result!=0)return 0;
  else{
    result = inet_pton(AF_INET6, test, &(sa.sin_addr));
    if(result != 0)return 0;
  }
  return -1;
}
int rand_a_b(int a, int b){
  time_t t;

  struct timespec start;
  clock_gettime(CLOCK_REALTIME,&start);
  srand((unsigned) time(&t));
  int nombre_aleatoire = (int)start.tv_nsec;
  //printf("Je suis dans rand %d\n",nombre_aleatoire%(b-a) +a);
  return 	nombre_aleatoire%(b-a) +a;
}

char * ip_libre_multi(){

  int p1=rand_a_b(0,255);
   int p2=rand_a_b(0, 255);
  int p3=rand_a_b(0, 255);
  char *buff=intchar( 226, 3);
  strcat(buff,".");
  char* t1 = malloc(sizeof(char)*4);
  sprintf(t1,"%d",p1);
  strcat(buff,t1);
  strcat(buff,".");
  
  sprintf(t1,"%d",p2);
  strcat(buff,t1);
  
  strcat(buff,".");
  sprintf(t1,"%d",p3);
  strcat(buff,t1);
  free(t1);
  return buff;
}

int port_libre_multi(char* ip){
  int sock=socket(PF_INET,SOCK_DGRAM,0);
  int ok=1;
  int r=setsockopt(sock,SOL_SOCKET,SO_REUSEPORT,&ok,sizeof(ok));
  struct sockaddr_in address_sock;
  address_sock.sin_family=AF_INET;
  int port=1024;
  int i;
  i=0;
  while( 1){
    port=rand_a_b(1024, 10000);
    address_sock.sin_port=htons(port);
    address_sock.sin_addr.s_addr=htonl(INADDR_ANY);
    r=bind(sock,(struct sockaddr *)&address_sock,sizeof(struct sockaddr_in));
    if(r==0||i==20){
      close(sock);
      break;
    } 
    else{i++; }
  }
  
  struct ip_mreq mreq;
  mreq.imr_multiaddr.s_addr=inet_addr(ip);
  mreq.imr_interface.s_addr=htonl(INADDR_ANY);
  r=setsockopt(sock,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
  return port;
}


/* Function that generate id of UDP messages */
char* gen_idmess(){
  time_t t;
  char* tmp = malloc(sizeof(char)*7);
  struct timespec start;
  int r = clock_gettime(CLOCK_REALTIME,&start);
  if(r==0){
    char idm[8];
    char* tab = "azertyuiopqsdfghjklmwxcvbn";
    srand((unsigned) time(&t));
    idm[0] = tab[rand()%26];
    int i = (int)start.tv_nsec;
    sprintf(tmp,"%d",i);
    //printf("Value of time %s\n",tmp);
    for (i = 1; i <8 ; i++)
    {
      if(strlen(tmp)>7){
        idm[i]=tmp[i+1];
        //printf("Value of i %c\n",tmp[i+1]);
      }else{
        idm[i]=tmp[i];
      }
    }
    char* idf = malloc(sizeof(char)*8);
    strcpy(idf,idm);
    //printf("Value of idm %s\n",idf);
    return idf;
  }
  return NULL;
}


/*Fonction convert int a into char* of size b if it is possible else return NULL*/
char* intchar(int a, int b){
  int i;
  char* tmp = malloc(sizeof(char)*b);
  sprintf(tmp,"%d",a);
  if(strlen(tmp)<b){
    char* res = malloc(sizeof(char)*b);
    for(i=0; i<strlen(tmp); i++){
      res[i]=tmp[i];
    }
    for(i=strlen(tmp); i<strlen(res); i++){
      res[i]='0';
    }
    free(tmp);
    return res;
  }else if(strlen(tmp)==b){
    return tmp;
  }
  return NULL;
}

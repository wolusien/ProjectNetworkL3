#include "tools.h"
#include "entity.h"


/*
  Function taking char* str and a char delim separator and split str in terms of separator
*/
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
    count_space += 2;
  }
  //printf("Val of count_space %d\n",count_space);
  char** tab = malloc(sizeof(char*)*(count_space));
  char* p = strtok(s,d);
  int j = 0;
  while(p!=NULL){
    tab[j] = malloc(sizeof(char)*strlen(p));
    strcpy(tab[j],p);
    j++;
    p = strtok(NULL,d);
  }
  tab[count_space-1]=NULL;
  return tab;
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
  //printf("ip à traiter %s\n",ip);
  if(strlen(ip)==9){
    if(ip[3]=='.' && '.'==ip[5] && '.'==ip[7]){
      char ip_final[15];
      bzero(ip_final,sizeof(char)*15);
      for (i = 0; i < 4; i++) {
        ip_final[i] = ip[i];
      }
      ip_final[4]='0';
      ip_final[5]='0';
      ip_final[6]=ip[4];
      ip_final[7]=ip[5];
      ip_final[8]='0';
      ip_final[9]='0';
      ip_final[10]=ip[6];
      ip_final[11]=ip[7];
      ip_final[12]='0';
      ip_final[13]='0';
      ip_final[14]=ip[8];
      char* ip_f = malloc(sizeof(char)*15);
      strcpy(ip_f,ip_final);
      return ip_f;
    }
  }else if(strlen(ip)==15){
    return ip;
  }
  return NULL;
}


/*
  Get ip adress knowing the host
*/
char* get_ip(char* host){
  struct hostent* h;
  h=gethostbyname(host);
  if(h==NULL){
    printf("get_ip : Unknown host given\n");
    return NULL;
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
    return host_addr;
  }
  return NULL;
}

/*
  Generate a unique code for id entity or application(normally)
*/
char* gen_code(){
  int i;
  char* id = malloc(sizeof(char)*8);
  char* tab = "a1ze0rty9uio2pqs8d@f6gh5jk3lm7wxc4vbn";
  time_t t;

  /* Intializes random number generator */
  srand((unsigned) time(&t));

  for (i = 0; i < 8; i++) {
    id[i] = tab[rand()%37];
    //printf("Value of rand %d\n" ,rand()%37);
  }
  return id;
}

/*
  Allow to get a free tcp port on a host
*/
int free_tport(char* host){
  int i;

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
    int port = 10000;

    for (i = 0; i < 65535-10000; i++) {
      port += i;
      adress_sock.sin_port = htons(port);
      inet_aton(host_addr,&adress_sock.sin_addr);
      int con = connect(sock, (struct sockaddr *)&adress_sock,
                        sizeof(struct sockaddr_in));
      if(con != 0){
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
int free_uport(char* host){
  int i;

  int sock = socket(PF_INET,SOCK_DGRAM,0);

  struct hostent* h;
  h=gethostbyname(host);
  if(h==NULL){
    printf("free_uport : Unknown host given\n");
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
    int port = 1;

    for (i = 0; i <= 9999; i++) {
      port += i;
      adress_sock.sin_port = htons(port);
      inet_aton(host_addr,&adress_sock.sin_addr);
      int b = bind(sock, (struct sockaddr *)&adress_sock,
                   sizeof(struct sockaddr_in));
      if(b == 0){
        return port;
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
  int i=0;
  char* test = malloc(sizeof(char)*9);
  if(strlen(ip)==15){
    for (i = 0; i < 4; i++)
    {
      test[i]=ip[i];
    }
    test[4]=ip[6];
    test[5]=ip[7];
    test[6]=ip[10];
    test[7]=ip[11];
    test[8]=ip[14];
  }else{
    test = ip;
  }
  struct sockaddr_in sa;
  int result = inet_pton(AF_INET, test, &(sa.sin_addr));
  if(result!=0)return 1;
  else{
    result = inet_pton(AF_INET6, test, &(sa.sin_addr));
    if(result != 0)return 1;
  }
  return -1;
}

int port_libre_multi(){
  int sock=socket(PF_INET,SOCK_DGRAM,0);
  sock=socket(PF_INET,SOCK_DGRAM,0);
  int ok=1;
  int r=setsockopt(sock,SOL_SOCKET,SO_REUSEPORT,&ok,sizeof(ok));
  struct sockaddr_in address_sock;
  address_sock.sin_family=AF_INET;
  int port=1024;
  int i;
  for( i =0;i<9999;i++){
    port=port+i;
    address_sock.sin_port=htons(9999);
    address_sock.sin_addr.s_addr=htonl(INADDR_ANY);
    r=bind(sock,(struct sockaddr *)&address_sock,sizeof(struct sockaddr_in));
    if(r==0)break;
  }

  struct ip_mreq mreq;
  mreq.imr_multiaddr.s_addr=inet_addr("225.1.2.4");
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


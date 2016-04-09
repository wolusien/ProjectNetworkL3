#include "tools.h"


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
  if(strlen(ip)==9){
    if(ip[3]=='.' && '.'==ip[5] && '.'==ip[7]){
      char* ip_final = malloc(sizeof(char)*15);
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
      return ip_final;
    }
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
    return ip_addZero(host_addr);
  }
  return NULL;
}

/*
Generate a unique code(normally)
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
    printf("Value of rand %d\n" ,rand()%37);
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
    int port = 9999;
    
    for (i = 0; i < 65535-9999; i++) {
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
    int port = 9999;
    
    for (i = 0; i < 65535-9999; i++) {
      port += i;
      adress_sock.sin_port = htons(port);
      inet_aton(host_addr,&adress_sock.sin_addr);
      int b = bind(sock, (struct sockaddr *)&adress_sock, 
                        sizeof(struct sockaddr_in));
      if(b != 0){
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
  struct sockaddr_in sa;
  int result = inet_pton(AF_INET, ip, &(sa.sin_addr));
  if(result!=0)return 1;
  else{
    result = inet_pton(AF_INET6, ip, &(sa.sin_addr));
    if(result != 0)return 1;
  }
  return -1;
}

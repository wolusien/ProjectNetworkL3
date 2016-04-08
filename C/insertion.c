 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "entity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
/*######################################################################################################################################################################################################################################################################################
                                                       PARTIE I:TRAITEMENT DE MESSAGE
#####################################################################################################################################################################################################################################################################################*/




char * concat(char *rep,char c){
 int len;
 len=strlen(rep);
 char *newstr=malloc(len+sizeof(char));
 sprintf(newstr, "%s%c", rep, c);
// printf("new chaine %s",newstr);
 return newstr;
 }
 int count_space(char *rep){
   int i;
   int space;
   space=0;
   
  // printf("rentre");
  for(i=0;i<strlen(rep);i++){
     //  printf("lettre %c",rep[i]);
    if( rep[i]==' ')space++;
  } 
  //  printf("nb espace %d",space);
  return space;
  
}
char ** splite(char *rep){
  printf("rentre2");
  int i;
    int taille;
    int index;
    index=0;
    taille=count_space(rep)+1;
    char ** split=malloc(taille*sizeof(char *));
    split[0]="";
    for(i=0;i<strlen(rep);i++){
      
      if( rep[i]==' ' ){
	printf("%s \n",split[index]);
	
	index++;
	split[index]="";
	
      }
      else split[index]=concat(split[index],rep[i]);
      split[index];
      
      
    }
    printf("%s\n",split[2]);
return split;
}

/*######################################################################################################################################################################################################################################################################################
                                                       PARTIE II:FONCTION D'INSERTION
#####################################################################################################################################################################################################################################################################################*/


/*###############################################################################################################################
fonction qui te genere une adresse ip
#####################################################################################################################################*/
char * get_adress_ip(char *buff){
  struct ifaddrs *myaddrs, *ifa;
  struct sockaddr_in *s4;
  int status;
  char *ip=(char *)malloc(64*sizeof(char));
  status = getifaddrs(&myaddrs);
  if (status != 0){
    perror("Probleme de recuperation d'adresse IP");
    exit(1);
  }
  for (ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next){
    if (ifa->ifa_addr == NULL) continue;
    if ((ifa->ifa_flags & IFF_UP) == 0) continue;
    if ((ifa->ifa_flags & IFF_LOOPBACK) != 0) continue;
    if (ifa->ifa_addr->sa_family == AF_INET){
      s4 = (struct sockaddr_in *)(ifa->ifa_addr);
      if (inet_ntop(ifa->ifa_addr->sa_family, (void *)&(s4->sin_addr),
		    ip, 64*sizeof(char)) != NULL){
	printf("Adresse IP :%s\n",ip);
	buff=malloc(strlen(ip)*sizeof(char));
	strcpy(buff,ip);
	printf("Adresse IP2 :%s\n",buff);
      }
    }
  }
  freeifaddrs(myaddrs);
  return buff;
}

/*###############################################################################################################################
fonction qui te trouve un port udp libre et l'associe a l'entitté
#####################################################################################################################################*/
int port_libre_udp(char *ip,entity *e){
  int sock=socket (PF_INET,SOCK_DGRAM,0);
  struct sockaddr_in adress_sock;
  adress_sock.sin_family=AF_INET;
  int i;
  int port;
  port=4242;
  for(i=0;i<800;i++){
    port++;
    adress_sock.sin_port=htons(port);
    adress_sock.sin_addr.s_addr=htonl(INADDR_ANY);
    int r= bind(sock,(struct sockaddr *)&adress_sock,sizeof(struct sock_addr_in *));
    if(r==0){
      e->prevudp_rport=port;
      return sock;//pas oublie de recupperer le port
    }
  }
}

/*###############################################################################################################################
conection au port tcp de l'entite princpal pour permettre l'insertion
#####################################################################################################################################*/
int connect_in_tcp(char *ip,int port,entity *e) {
    printf("port tester %s\n",ip);

    struct sockaddr_in adress_sock;
    adress_sock.sin_family=AF_INET;
    int i;
    adress_sock.sin_port=htons(port);
    inet_aton(ip,&adress_sock.sin_addr);
    int descr=socket (PF_INET,SOCK_STREAM,0);
    int r= connect(descr,(struct sockaddr *)&adress_sock,sizeof(struct sock_addr_in *));
    printf("port tester %d\n",port);
    printf("port tester %d\n",r);
    if(r==-1){
      char buff[100]; 
      int recu=recv(descr,buff,99*sizeof(char),0);
      buff[recu]='\0';
char *id;

      char *ip2=get_adress_ip(id);
      int port2=port_libre_udp(ip2,e);
      char *char_port;
char_port=malloc(sizeof(char)*10); 
//  sprintf(char_port, "%d", port2);
      //envoyer ip et port udp
      // char *ip2=get_adress_ip(ip2);
      char *mess="";//mettre un message de la form new ..
//mess=strcat("NEW ",ip2);
//mess=strcat(mess,char_port);
 write(descr,mess,strlen(mess)*sizeof(char));
    
      
//    e->tcp_port=port;
      return descr;//pas oublie de reccuperer le num de port d'ecoute
      
    }
}

/*###############################################################################################################################
fonction qui associe  un port  servant de tcp serveur à une entity
#####################################################################################################################################*/
int main_entity_server(entity *e){
  char *ip;
    ip=get_adress_ip(ip);
 int udp=port_libre_udp(ip,e);

//int tcp= connect_in_tcp(ip,4242,e);//changer 
    e->prevudp_rport=udp;
      e->nextudp_sport=udp;
// e->tcp_port=tcp;
      e->ip_ad=ip;
      //regler les multi diff

printf("ip-server %s",ip);
printf("port-server %d",udp);
int sock=socket(PF_INET,SOCK_STREAM,0);
struct sockaddr_in address_sock;
address_sock.sin_family=AF_INET;
address_sock.sin_port=htons(4245);
address_sock.sin_addr.s_addr=htonl(INADDR_ANY);
int r=bind(sock,(struct sockaddr *)&address_sock,sizeof(struct
sockaddr_in));
printf("r %d",r);
if(r==0){
r=listen(sock,0);
while(1){
struct sockaddr_in caller;
socklen_t size=sizeof(caller);
int sock2=accept(sock,(struct sockaddr *)&caller,&size);
if(sock2>=0){
  char *mess="hello";//mettre un message de la form welc ..
  write(sock2,mess,strlen(mess)*sizeof(char));
  char buff[100];
  int recu=recv(sock2,buff,99*sizeof(char),0);
  buff[recu]='\0';
  if (strcmp(buff,"ACKC\n"))close(sock2);
}   
close(sock2);
}
}
return 0;

}



int main (int argc, char *argv[]){

// splite("yes we can"); 
if(argc==1){
printf("mode serveur\n");
char *buff;
  buff= get_adress_ip(buff);
printf("ip attribuer %s",buff);
//entity *e= malloc(sizeof(entity));
//main_entity_server(e);
//int r;
//char *ip="127.0.0.1";
//r= port_libre(ip,e);  
//printf("port attribuer %d",r);
// close(r);

}
 else{
printf("mode client\n");
entity *e2=malloc(sizeof(entity));
connect_in_tcp("192.168.0.19",4243,e2);

}

  
}

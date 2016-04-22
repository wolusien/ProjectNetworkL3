#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>


typedef struct message{
  char ip[15];
  char ip_diff[15];
  char ip_succ[15];
  char port[15];    
char port_diff[15];        
char port_succ[15];      
char idm[15];     
char  ip_app[15];
char  id [15];
char  size_nom[15];char  size_mess[15];
char  size_content[15];
char  num_mess[15];
}message;

entity* init_entity(char id[8], int prevudp_lport, int tcp_port, 
                    char* ip_ad, int nextudp_lport, char* ipv4_ad, int cast_port);

entity* init_ring(char id[8], int udp_port, int tcp_port, 
char* ip_ad, char* ipv4_ad, int cast_port);


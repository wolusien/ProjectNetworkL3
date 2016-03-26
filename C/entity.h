#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>


typedef struct entity{
  char id[8];
  int prevudp_rport;
  int tcp_port;
  char* ip_ad;
  int nextudp_sport;
  char* ipv4_ad;
  int cast_port;
}entity;

entity* init_entity(char id[8], int prevudp_lport, int tcp_port, 
char* ip_ad, int nextudp_lport, char* ipv4_ad, int cast_port);



#include "entity.h"


entity* init_entity(char id[8], int prevudp_lport, int tcp_port, 
                    char* ip_ad, int nextudp_lport, char* ipv4_ad, int cast_port){
  entity* ent = malloc(sizeof(entity));
  (*ent).id = id;
  (*ent).prevudp_rport = prevudp_rport;
  (*ent).tcp_port = tcp_port;
  (*ent).ip_ad = malloc(sizeof(char)*strlen(ip_ad));
  (*ent).ip_ad = ip_ad;
  (*ent).ipv4_ad = malloc(sizeof(char)*strlen(ipv4_ad));
  (*ent).ipv4_ad = ipv4_ad;
  (*ent).cast_port = cast_port;
  return ent;
}

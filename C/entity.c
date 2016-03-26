#include "entity.h"

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
  Function that create a struct entity containing information about an entity wich will be use for insertion or other
 */
entity* init_entity(char id[8], int prevudp_rport, int tcp_port, 
                    char* ip_ad, int nextudp_sport, char* ipv4_ad, int cast_port){
  if(id!=NULL && ip_ad!=NULL 
&& strlen(ip_ad)==15 
&& ipv4_ad!=NULL){
    if(prevudp_rport>9999 && prevudp_rport<65535 
&& tcp_port>9999 tcp_port<65535
&& nextudp_sport>9999 && nextudp_sport<65535
&& cast_port>9999 && cast_port<65535){
      if(cast_port!=nextudp_sport && cast_port!=prevudp_rport && cast_port!= tcp_port 
&& prevudp_rport != nextudp_sport && prevudp_rport!=tcp_port && nextudp_sport != tcp_port ){
        entity* ent = malloc(sizeof(entity));
        (*ent).id = id;
        (*ent).prevudp_rport = prevudp_rport;
        (*ent).tcp_port = tcp_port;
        (*ent).ip_ad = ip_ad;
        (*ent).nextudp_sport = nextudp_sport;
        (*ent).ipv4_ad = ipv4_ad;
        (*ent).cast_port = cast_port;
        return ent;
      }
    }
  }
  return NULL;
}


#include "entity.h"


/*!Function which initialize an entity
 *\param char id[8] id of the entity
 *\param int my_uport udp port of the entity
 *\param int tcp_port  tcp port of the enity
 *\param char* ip_ad ip address of the entity
 *\param int next_uport udp por of the next entity
 *\param char* ipv4_ad ipv4 adress of broadcast
 *\param int cast_port tcp port of broadcast
 *\return entity* pointer to an entity created
 */
entity* init_entity(char id[8], int my_uport, int tcp_port, 
                    char* ip_ad, int next_uport, char* ipv4_ad, int cast_port){
  if(id!=NULL && ip_ad!=NULL 
&& strlen(ip_ad)==15 
&& ipv4_ad!=NULL){
    if(my_uport>9999 && my_uport<65535 
&& tcp_port>9999 tcp_port<65535
&& next_uport>9999 && next_uport<65535
&& cast_port>9999 && cast_port<65535){
      if(cast_port!=next_uport && cast_port!=my_uport && cast_port!= tcp_port 
&& my_uport != next_uport && my_uport!=tcp_port && next_uport != tcp_port ){
        entity* ent = malloc(sizeof(entity));
        (*ent).id = id;
        (*ent).my_uport = my_uport;
        (*ent).tcp_port = tcp_port;
        (*ent).ip_ad = ip_ad;
        (*ent).next_uport = next_uport;
        (*ent).ipv4_ad = ipv4_ad;
        (*ent).cast_port = cast_port;
        return ent;
      }
    }
  }
  return NULL;
}


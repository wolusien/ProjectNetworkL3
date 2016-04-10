#include "tools.h"

typedef struct entity{
  char* id;
  char* my_ip;
  int my_uport;
  int tcp_port;
  char* next_ip;
  int next_uport;
  char* cast_ip;
  int cast_port;
}entity;

/*
Function which initialize an entity
 */
int init_entity(entity* ent, char* host);

/*
Function that manage the insertion
*/
int insertion(entity* e, char* host, int tcp_e1);

/*
Function that manage insertion(server part)
*/
int serv_tcp(entity* e);

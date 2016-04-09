#include "tools.h"

typedef struct entity{
  char id[8];
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

int insertion(entity* e, char* host, int tcp_e1);

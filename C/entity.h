#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "tools.h"

typedef struct entity{
  char id[8];
  char* my_ip;
  int my_uport;
  int tcp_port;
  int prev_uport;
  char* next_ip;
  int next_uport;
  char* cast_ip;
  int cast_port;
}entity;

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
int init_entity(entity* ent, char id[8],char* my_ip, int my_uport, int tcp_port, int prev_uport, char* next_ip, int next_uport, char* cast_ip, int cast_port);

int insertion(entity* e, char* host, int tcp_e1);

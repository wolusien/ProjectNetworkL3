#include <pthread.h>
#include "tools.h"

typedef struct entity{
  char id[8];
  /*Contains before the insertion the name of the host machine of
   * previous entity in the ring where it will be insert
   */
  char* my_ip;
  //<=9999 and >0
  int my_uport;
  /*Contains before the insertion the tcp port of 
   * previous entity in the ring where it will be insert
   */
  int tcp_port;

  int nb_insert;
  
  char* next_ip1;
  //<=9999 and >0
  int next_uport1;
  char* cast_ip1;
  //<=9999 and >0
  int cast_port1;
  
  
  char* next_ip2;
  //<=9999 and >0
  int next_uport2;
  char* cast_ip2;
  //<=9999 and >0
  int cast_port2;

}entity;

/*
Function which initialize an entity
 */
int init_entity(entity* ent, char* host);

/*
Function that manage the insertion
*/
int insertion(entity* e, char* host, int tcp_e1);

/*Function for using thread for the insertion
 */
void* pth_insertion(void* arg);

/*
Function that manage insertion(server part)
*/
int serv_tcp(entity* e);

/*Function for using thread for the server tcp
 */
 void* pth_tserv(void* arg);
 
/*Function for dupliaction into a ring
 */
int duplication(entity* e, char* host, int e1_tcp);

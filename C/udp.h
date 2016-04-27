#include "entity.h"

/* Struct for manage the udp messages */
typedef struct uEntity{
  entity* ent;
  /* Number of case in the array rec */
  int rec_size;
  /* First case in the array rec which is NULL */
  int rec_pos;
  char** rec;
  /* Number of case in the array env */
  int env_size;
  /* First case in the array env which is NULL */
  int env_pos;
  char** env;

  /* Id of application host by an entity */
  char id_app[8];
}uEntity;


/*Function adding a char* mess to an array of an uEntity
 * depends on option given : 0 for env array
 *                           1 for rec array
 */
int add_umess(uEntity* u, int option, char* mess);

/*Check if a message has been seen yet
*If it is on array env return 0
*If it is on array rec return 1
*Else return -1
*/
int isin(uEntity* u, char* idm);

/* Function initialising structure app_message */
//int init_appmess(app_message* m, char* id_app, char* mess);

/* Function for initialising an entity */
int init_uEntity(uEntity* u, char* host);


/*Global fonction for udp message*/
void* rec_udp(void* u);

/*Manage application message*/
int app_mess(uEntity* u, char* buff);


/*Manage WHOS message*/
int whos(uEntity* u, char* buff);

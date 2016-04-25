#include "entity.h"

/* Struct for manage the udp messages */
typedef struct uEntity{
  entity* ent;
  /* Number of case in the array rec */
  int rec_size = 100;
  /* First case in the array rec which is NULL */
  int rec_pos = 0;
  char** rec = malloc(sizeof(char*)*rec_size);
  /* Number of case in the array env */
  int env_size = 100;
  /* First case in the array env which is NULL */
  int env_pos = 0;
  char** env = malloc(sizeof(char*)*env_size);

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
int init_appmess(app_message* m, char* id_app, char* mess);

/* Function for initialising an entity */
int init_uEntity(uEntity* u, entity* e);


/*Global fonction for udp message*/
int rec_udp(uEntity* u);

/*Manage application message*/
int app_mess(char* buff, uEntity* u);


/*Manage WHOS message*/
int whos(uEntity* u, char* buff);

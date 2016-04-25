#include "entity.h"

/* Struct for manage the udp messages */
typedef struct uEntity{
  entity* ent;
  /* Number of case in the array rec */
  int rec_size = 100;
  /* First case in the array rec which is NULL */
  int rec_pos = 0;
  int* rec = malloc(sizeof(int)*rec_size);
  /* Number of case in the array env */
  int env_size = 100;
  /* First case in the array env which is NULL */
  int env_pos = 0;
  int* env = malloc(sizeof(int)*env_size);

  /* Id of application host by an entity */
  char id_app[8];
}uEntity;

/* Function initialising structure app_message */
int init_appmess(app_message* m, char* id_app, char* mess);

/* Function for initialising an entity */
int init_uEntity(uEntity* u, entity* e);

/*Check if a message has been received yet*/
int isin(uEntity* u, int ptr);

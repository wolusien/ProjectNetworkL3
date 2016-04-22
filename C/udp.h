#include "entity.h"

/* Struct for manage the udp messages */
typedef struct uEntity{
  entity* ent;
  /* Number of column in the array rec */
  int rec_size = 100;
  /* Last column in the array rec which is not NULL */
  int rec_pos = 0;
  int* rec = malloc(sizeof(int)*rec_size);
  /* Number of column in the array env */
  int env_size = 100;
  /* Last column in the array env which is not NULL */
  int env_pos = 0;
  int* env = malloc(sizeof(int)*env_size);
}uEntity;

/* Function that generate id of UDP messages */
char* gen_idmess();

/* Function initialising structure app_message */
int init_appmess(app_message* m, char* id_app, char* mess);

/* Function for initialising an entity */
int init_uEntity(uEntity* u, entity* e);

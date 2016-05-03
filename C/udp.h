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
  char* id_app;
  
  //Compteur de temps
  double count_time1;
  double count_time2;
  int down1;
  int down2;
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
int init_uEntity(uEntity* u);

////////////////////////////////////////////////////////////////////////
/////////////////////UDP RECEPTION//////////////////////////////////////

/* Function managing application udp message protocol
 */
int app_mess(uEntity* u, char* buff);

/* Function managing WHOS udp message protocol
 */
int whos(uEntity* u, char* buff);

/* Function managing GBYE udp message protocol
 */
int gbye(uEntity* u, char* buff);

/* Function managing TEST udp message protocol
 */
int testring(uEntity* u, char* buff);


/*Global fonction for udp message*/
void* rec_udp(void* u);


//////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////UDP ENVOI////////////////////////////////////////////////////////

int gen_appmess(uEntity* u, char* mess);

int gen_whosmess(uEntity* u);

int gen_gbyemess(uEntity* u, int ring);

char* gen_testmess(uEntity* u, int ring);

void* envoi_udp(void* e);

void* gentest_udp(void* e);




void* rec_multi_udp(void* uent);

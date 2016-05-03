#include "udp.h"


int main(int argc, char *argv[])
{
  uEntity* u= malloc(sizeof(uEntity));
  int r = init_uEntity(u);
  if (r==0) {
    (*u).ent->my_ip = argv[1];
    (*u).ent->my_uport = atoi(argv[2]);
    (*u).ent->cast_port1 = atoi(argv[2]);
    (*u).ent->cast_ip1 = argv[3];
    (*u).id_app = "12345679";
    (*u).ent->next_uport1 = 1025;
    pthread_t th2;
    pthread_create(&th2,NULL,rec_multi_udp,u);
    pthread_join(th2,NULL);
  }else{
    printf("Problem with init entity\n");
  }
  return 0;
}

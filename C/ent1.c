#include "udp.h"


int main(int argc, char *argv[])
{
  /*
  entity* ent = malloc(sizeof(entity));
  int r2 = init_entity(ent, "localhost");
  if (r2==0) {
    (*ent).my_ip = "localhost";
    (*ent).tcp_port = 50001;
    pthread_t th2;
    pthread_create(&th2,NULL,pth_insertion,ent);
    pthread_join(th2,NULL);
  }else{
    printf("Problem with init entity\n");
  }
  */
  uEntity* u= malloc(sizeof(uEntity));
  int r = init_uEntity(u);
  if (r==0) {
    (*u).ent->my_ip = argv[1];
    (*u).ent->tcp_port = atoi(argv[2]);
    (*u).ent->my_uport = 1028;
    (*u).ent->next_uport1 = 1027;
    gen_whosmess(u);
    /*
    pthread_t th2;
    pthread_t th3;
    pthread_create(&th2,NULL,pth_insertion,(*u).ent);
    pthread_create(&th3,NULL,rec_udp,u);
    pthread_join(th2,NULL);
    pthread_join(th3,NULL);
    */
    
  }else{
    printf("Problem with init entity\n");
  }
  return 0;
}

#include "entity.h"


int main(int argc, char *argv[])
{
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
  return 0;
}

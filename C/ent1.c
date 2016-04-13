#include "entity.h"


int main(int argc, char *argv[])
{
  entity* e = malloc(sizeof(entity));
  int r = init_entity(e, "localhost");
  if (r==0) {
    (*e).my_ip = "localhost";
    (*e).tcp_port = 50000;
    pthread_t th1;
    pthread_create(&th1,NULL,pth_insertion,e);
    pthread_join(th1,NULL);
  }else{
    printf("Problem with init entity\n");
  }
  return 0;
}

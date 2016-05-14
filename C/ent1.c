#include "udp.h"


int main(int argc, char *argv[])
{
  uEntity* u= malloc(sizeof(uEntity));
  int r = init_uEntity(u);
  if (r==0) {
    (*u).ent->my_ip = argv[1];
    (*u).ent->tcp_port = atoi(argv[2]);
    (*u).id_app = "12345979";
    pthread_t th1;
    pthread_create(&th1,NULL,pth_insertion,(*u).ent);
    pthread_t th2;
    pthread_create(&th2,NULL,rec_udp,u);
    pthread_t th3;
    pthread_create(&th3,NULL,envoi_udp,u);
    pthread_t th4;
    pthread_create(&th4,NULL,rec_multi_udp,u);
    pthread_join(th1,NULL);
    pthread_join(th2,NULL);
    pthread_join(th3,NULL);
    pthread_join(th4,NULL);
  }else{
    printf("Problem with init entity\n");
  }
  return 0;
}

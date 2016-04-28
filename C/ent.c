#include "udp.h"


int main(int argc, char *argv[])
{
  entity* e = malloc(sizeof(entity));
  int r = init_entity(e, "localhost");
  if (r==0) {
    (*e).my_ip = "127.0.0.1";
    (*e).tcp_port = 50001;
    //printf("Value of (*e).tcp_port %d\n",(*e).tcp_port);
    pthread_t th1;
    pthread_create(&th1,NULL,pth_tserv,e);
    pthread_join(th1,NULL);
  }else{
    printf("Problem with init entity\n");
  }
  
  /*
  char* test = "128.0.8.1";
  char** tab = split(test, '.');
  printf("Value of split %s\n",tab[3]);
  printf("Value of ip %s\n",ip_addZero("127.54.9.1"));
  */
  
  /*
  uEntity* u = malloc(sizeof(uEntity));
  int res = init_uEntity(u,"localhost");
  if (res==0)
  {
    (*u).ent->my_ip = "127.000.000.001";
    (*u).ent->my_uport = 1700;
    (*u).ent->next_uport1 = 1800;
    (*u).id_app = "12345698";
    (*u).ent->cast_ip1 = "127.000.000.001";
    (*u).ent->cast_port1 = 2000;
    rec_udp(u);
  }
  */
  return 0;
}
 

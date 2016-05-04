#include "udp.h"


int main(int argc, char *argv[])
{
  /*
  entity* e = malloc(sizeof(entity));
  int r = init_entity(e);
  if (r==0) {
    (*e).my_ip = "127.0.0.1";
    (*e).tcp_port = 50001;
    //printf("Value of (*e).tcp_port %d\n",(*e).tcp_port);
    pthread_t th1;
    pthread_create(&th1,NULL,pth_tserv,e);
    //pthread_join(th1,NULL);
    

    entity* ent = malloc(sizeof(entity));
    int r2 = init_entity(ent);
    if (r2==0) {
      (*ent).my_ip = "127.0.0.1";
      (*ent).tcp_port = 50001;
      pthread_t th2;
      //pthread_create(&th1,NULL,pth_tserv,e);
      pthread_create(&th2,NULL,pth_insertion,ent);
      pthread_join(th1,NULL);
      pthread_join(th2,NULL);
    }
  }*/
  uEntity* u= malloc(sizeof(uEntity));
  int r = init_uEntity(u);
  if (r==0) {
    //(*u).ent->my_uport = 1025;
    //(*u).ent->next_ip1 = (*u).ent->my_ip;
    //(*u).ent->next_uport1 = 1030;
    (*u).id_app = "12345679";
    /*
    gen_whosmess(u);
    gen_appmess(u,"SA marche ou pas");
    gen_gbyemess(u,1);
    */
    pthread_t th1;
    pthread_create(&th1,NULL,envoi_udp,u);
    pthread_t th2;
    pthread_create(&th2,NULL,pth_tserv,(*u).ent);
    pthread_t th3;
    pthread_create(&th3,NULL,rec_udp,u);
    pthread_join(th1,NULL);
    pthread_join(th2,NULL);
    pthread_join(th3,NULL);
  }
  return 0;
}
 

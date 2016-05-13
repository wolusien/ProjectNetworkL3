#include "udp.h"

int main(int argc, char *argv[])
{
  uEntity* u= malloc(sizeof(uEntity));
  int r = init_uEntity(u);
  if (r==0) {
    //(*u).ent->my_uport = 1030;
    //(*u).ent->next_ip1 = (*u).ent->my_ip;
    //(*u).ent->next_uport1 = 1030;
    (*u).id_app = "12345679";
    /*
    gen_whosmess(u);
    gen_appmess(u,"SA marche ou pas");
    gen_gbyemess(u,1);
    */
    pthread_t th1;
    pthread_create(&th1,NULL,pth_dupl,(*u).ent);
    pthread_t th2;
    pthread_create(&th2,NULL,envoi_udp,u);
    pthread_t th3;
    pthread_create(&th3,NULL,rec_udp,u);
    pthread_t th4;
    pthread_create(&th4,NULL,rec_multi_udp,u);
    pthread_join(th1,NULL);
    pthread_join(th2,NULL);
    pthread_join(th3,NULL);
    pthread_join(th4,NULL);
  }
  return 0;
}

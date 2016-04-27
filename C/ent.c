#include "udp.h"


int main(int argc, char *argv[])
{
  /*
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
  */
  uEntity* u = malloc(sizeof(uEntity));
  int res = init_uEntity(u,"localhost");
  if (res==0)
  {
    (*u).ent->my_ip = "127.000.000.001";
    (*u).ent->my_uport = 1700;
    (*u).ent->next_uport1 = 1800;
    
    /*int sock=socket(PF_INET,SOCK_DGRAM,0);
    sock=socket(PF_INET,SOCK_DGRAM,0);
    struct sockaddr_in address_sock;
    address_sock.sin_family=AF_INET;
    address_sock.sin_port=htons(1700);
    address_sock.sin_addr.s_addr=htonl(INADDR_ANY);
    int r=bind(sock,(struct sockaddr *)&address_sock,sizeof(struct
                                                            sockaddr_in));
    if(r==0){
      char tampon[100];
      while(1){
        int rec=recv(sock,tampon,100,0);
        tampon[rec]='\0';
        printf("Message recu : %s\n",tampon);*/
        rec_udp(u);
      //}
    //}
  }
  return 0;
}
 

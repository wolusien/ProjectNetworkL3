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
  uEntity* u = malloc(sizeof(uEntity));
  int res = init_uEntity(u,"localhost");
  if (res==0)
  {
    (*u).ent->my_uport = 1800;
    (*u).ent->next_uport1 = 1700;
    int sock=socket(PF_INET,SOCK_DGRAM,0);
    struct addrinfo *first_info;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype=SOCK_DGRAM;
    int r=getaddrinfo("localhost","1700",&hints,&first_info);
    if(r==0){
      if(first_info!=NULL){
        struct sockaddr *saddr=first_info->ai_addr;
        char* tampon = "WHOS 12345678";
        sendto(sock,tampon,strlen(tampon),0,saddr,
               (socklen_t)sizeof(struct sockaddr_in));
        printf("L'envoi a été fait %s\n",tampon);
      }
    }
    rec_udp(u);
  }
  return 0;
}

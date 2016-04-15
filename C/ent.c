#include "entity.h"


int main(int argc, char *argv[])
{
  /*int sock=socket(PF_INET,SOCK_STREAM,0);
  struct sockaddr_in address_sock;
  
  address_sock.sin_family = AF_INET;
  address_sock.sin_port = htons(atoi(argv[1]));
  inet_aton("127.0.0.1",&address_sock.sin_addr);
  
  address_sock.sin_family=AF_INET;
  address_sock.sin_port=htons(atoi(argv[1]));
  address_sock.sin_addr.s_addr=htonl(INADDR_ANY);
  
  int r=bind(sock,(struct sockaddr *)&address_sock,sizeof(struct sockaddr_in));
  if(r==0){
    r=listen(sock,0);
    while(1){
      struct sockaddr_in caller;
      socklen_t size=sizeof(caller);
      int sock2=accept(sock,(struct sockaddr *)&caller,&size);
      if(sock2>=0){
        char *mess="WELC 127.000.000.001 48000 127.000.000.001 48001\n";
        send(sock2,mess,strlen(mess)*sizeof(char),0);
        char buff[100];
        int recu=recv(sock2,buff,99*sizeof(char),0);
        buff[recu]='\0';
        printf("Message recu : %s\n",buff);
        char* m = "ACKC\n";
        send(sock2,m,strlen(m)*sizeof(char),0);
      }
      close(sock2);
    }
  }*/
  
  entity* e = malloc(sizeof(entity));
  int r = init_entity(e, "localhost");
  if (r==0) {
    (*e).my_ip = "127.0.0.1";
    (*e).tcp_port = 50000;
    //printf("Value of (*e).tcp_port %d\n",(*e).tcp_port);
    pthread_t th1;
    pthread_create(&th1,NULL,pth_tserv,e);
    //pthread_join(th1,NULL);
  }else{
    printf("Problem with init entity\n");
  }
  
  entity* ent = malloc(sizeof(entity));
  int r2 = init_entity(ent, "localhost");
  if (r2==0) {
    (*ent).my_ip = "localhost";
    (*ent).tcp_port = 50000;
    pthread_t th2;
    pthread_create(&th2,NULL,pth_insertion,ent);
    //pthread_join(th2,NULL);
  }else{
    printf("Problem with init entity\n");
  }
  return 0;
}

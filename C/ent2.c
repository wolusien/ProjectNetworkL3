#include "udp.h"

int main(int argc, char *argv[])
{
  uEntity* u= malloc(sizeof(uEntity));
  int r = init_uEntity(u);
  if (r==0) {
    duplication((*u).ent,argv[1],atoi(argv[2]));
  }else{
    printf("Problem with init entity\n");
  }
  return 0;
}

#include "entity.h"


int main(int argc, char *argv[])
{
  entity* ent = malloc(sizeof(entity));
  int r = init_entity(ent, "12345678", "127.000.000.001", 50000, 50001, 50000, "127.000.000.001", 50002,"127.000.000.001",50002);
  if (r==0) {
    printf("Value of (*ent).my_uport %d\n" ,(*ent).my_uport);
    insertion(ent,"localhost",49000);
  }else{
    printf("Probleme avec init_entity\n");
  }
  return 0;
}

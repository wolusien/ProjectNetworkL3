#include "entity.h"


int main(int argc, char *argv[])
{
  entity* e1 = init_entity("12345678",50000,50001,"127.000.000.001",50002,"127.000.000.001",50002);
  insert_entity(e1,"localhost",49000);
  return 0;
}

 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "entity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
char ** splite(char rep);
int count_space(char *rep);
char * concat(char *rep,char c);

char * get_adress_ip(char *buff);

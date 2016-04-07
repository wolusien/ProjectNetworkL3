#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

/*
Function taking char* str and a char delim separator and split str depending on separator
 */
char** split(char* str, char delim);

/*
Allow to correct ip coded on 9o to 15o
 */
char* ip_addZero(char* ip);

/*
Function act same as split in JAVA for String
 */
int str_arrsize(char** tab);


/*
Function that allow to get free tcp port for an host
*/
int free_tport(char* host);

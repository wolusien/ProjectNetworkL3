#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
Function act same as split in JAVA for String
 */
int str_arrsize(char** tab);

/*
Allow to correct ip coded on 9o to 15o
 */
char* ip_addZero(char* ip);

/*
Get ip adress knowing the host
*/
char* get_ip(char* host);

/*
Generate a unique code(normally)
*/
char* gen_code();

/*
Function that allow to get free tcp port for an host
*/
int free_tport(char* host);


/*
Function that allow to get free udp port for an host
*/
int free_uport(char* host);

/*
Function that verify an ip_address
 */
int check_ip(char* ip);

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
#include <ifaddrs.h>
#include <linux/if_link.h>
#include <sys/ioctl.h>
#include <net/if.h>



/*
Function taking char* str and a char delim separator and split str depending on separator
 */
char ** split(char *rep, char delim);

/*Count the number of delim in char* rep
 */
int count_space(char *rep,char delim);

/*Concatenation of a char* rep and a char c
 */
char * concat(char *rep,char c);

/*
Function act same as split in JAVA for String
 */
int str_arrsize(char** tab);

/*
Allow to correct ip coded on 9o to 15o
 */
char* ip_addZero(char* ip);

/*
Allow to transform ip coded on 15o to 9o
 */
char* ip_removeZero(char* ip);

/*
Get ip adress knowing the host
*/
char* get_ip();

/*
Generate a unique code(normally)
*/
char* gen_code();

/*
Function that allow to get free tcp port for an host
*/
int free_tport();


/*
Function that allow to get free udp port for an host
*/
int free_uport();

/*
Function that verify an ip_address
 */
int check_ip(char* ip);


/* Function that generate id of UDP messages */
char* gen_idmess();


/*Fonction convert int a into char* of size b if it is possible else return NULL*/
char* intchar(int a, int b);


int rand_a_b(int a, int b);

char * ip_libre_multi();

int port_libre_multi();

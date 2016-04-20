#include "tool.h"
#include <netinet/in.h>
#include <arpa/inet.h>


/*!Function taking char* str and a char delim separator and split str in terms of separator
 *\param char* str char* to split
 *\param char delim separator of the tokens
 *\return Char** array which will contains the token gotten by the split
 */
int count_space(char *rep){
   int i;
   int space;
   space=0;
   
  // printf("rentre");
  for(i=0;i<strlen(rep);i++){
     //  printf("lettre %c",rep[i]);
    if( rep[i]==' ')space++;
  } 
  //  printf("nb espace %d",space);
  return space;
  
}
char** split(char* str, char delim) {
  int i = 0;
  int count_space = 0;
  char* s = malloc(sizeof(char)*strlen(str));
  strncpy(s,str,strlen(str));
  char* d = &delim;
  for (i = 0; i < strlen(str); i++) {
    if(str[i] == delim){
      count_space++;
    }
  }
  if(count_space >0){
    count_space += 2;
  }
  //printf("Val of count_space %d\n",count_space);
  char** tab = malloc(sizeof(char*)*(count_space));
  char* p = strtok(s,d);
  int j = 0;
  while(p!=NULL){
    tab[j] = malloc(sizeof(char)*strlen(p));
    strcpy(tab[j],p);
    j++;
    p = strtok(NULL,d);
  }
  tab[count_space-1]=NULL;
  return tab;
}

int str_arrsize(char** tab){
  int count = 0;
  while(tab[count]!=NULL){
    count++;
  }
  return count;
}


/*
Function taking ip address of form "127.0.0.1" and change it to "127.000.000.001"
*/
char* ip_addZero(char* ip){
  int i;
  if(strlen(ip)==9){
    if(ip[3]=='.' && '.'==ip[5] && '.'==ip[7]){
      char* ip_final = malloc(sizeof(char)*15);
      for (i = 0; i < 4; i++) {
        ip_final[i] = ip[i];
      }
      ip_final[4]='0';
      ip_final[5]='0';
      ip_final[6]=ip[4];
      ip_final[7]=ip[5];
      ip_final[8]='0';
      ip_final[9]='0';
      ip_final[10]=ip[6];
      ip_final[11]=ip[7];
      ip_final[12]='0';
      ip_final[13]='0';
      ip_final[14]=ip[8];
      return ip_final;
    }
  }
  return NULL;
}
int verif_mess_server(char *mess){
  char **tab=split(mess,' ');
  if(count_space(mess)+1!=5)return-1;
    printf("%d\n",count_space(mess));
    if(strcmp(tab[0],"WELC")!=0)return-1;
    return 1;
}
//tester
int verif_mess_entrant(char *mess,entity *e){
  char **tab=split(mess,' ');
  if(count_space(mess)+1!=3)return-1;
    if(strcmp(tab[0],"NEWC")!=0) return-1;
    // e->myport=
    // e->ip=
    
    return 1;
}

































int main(){
  int r;
 struct in_addr address;
 inet_aton(" 127.0.0.12DF" , &address);




















printf("est une address  %d\n",r);
  r=verif_mess_server("WELC 127.0.0.1 5432 127.0.0.1 5431");

printf("%d\n",r);
}

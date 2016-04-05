#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*!Function taking char* str and a char delim separator and split str in terms of separator
 *\param char* str char* to split
 *\param char delim separator of the tokens
 *\return Char** array which will contains the token gotten by the split
 */
char** split(char* str, char delim);

char* ip_addZero(char* ip);

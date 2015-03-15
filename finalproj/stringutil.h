#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#ifndef __SHARED_INCLUDED
#define __SHARED_INCLUDED
 
void trim(char *str);
void chopFront(char *string);
void chopFrontCount(char *string, int count);
unsigned int parseInt(char *string);
int countHits(char *string, char letter);
int getlineFromString(char *buffer, char *input);
int startsWith(char *input, char *target);
int endsWith(char *ending, char *target);
int readUntilChar(char *buffer, char *input, char letter);
char *getDateStamp();
char *getCurrentDir();

#endif // __INTERFACE_INCLUDED

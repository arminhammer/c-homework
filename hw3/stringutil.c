//From http://en.wikipedia.org/wiki/Trimming_(computer_programming)

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void rtrim(char *str)
{
  size_t n;
  n = strlen(str);
  while (n > 0 && isspace((unsigned char)str[n - 1])) {
    n--;
  }
  str[n] = '\0';
}
 
void ltrim(char *str)
{
  size_t n;
  n = 0;
  while (str[n] != '\0' && isspace((unsigned char)str[n])) {
    n++;
  }
  memmove(str, str + n, strlen(str) - n + 1);
}
 
void trim(char *str)
{
  rtrim(str);
  ltrim(str);
}

void chopFront(char *string) {
	if (string[0] != '\n') {
	    memmove(string, string+1, strlen(string));
	}
}

/*
 * Parse a string as an integer
 */
unsigned int parseInt(char *string) {
	char *ptr = string;
	unsigned int val = strtoul(string, &ptr, 10);

	return val;
}

/*
 * Count the number of occurrences of a character in a string
 */
int countHits(char *string, char letter) {
	int i;
	int count = 0;
	for(i=0; i < strlen(string); i++) {
		if(string[i] == letter) count++;
	}
	return count;
}


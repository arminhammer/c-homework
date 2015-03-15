//From http://en.wikipedia.org/wiki/Trimming_(computer_programming)

#include <string.h>
#include <ctype.h>
 
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

int Minimum(int a, int b, int c) {
  int min = a;
  if(a > b) min = b;
  if(min > c) min = c;
  return min;
}

int LevenshteinDistance(char *s1, char *s2) {
  unsigned int s1len, s2len, x, y, lastdiag, olddiag;
  s1len = strlen(s1);
  s2len = strlen(s2);
  unsigned int column[s1len+1];
  for (y = 1; y <= s1len; y++)
    column[y] = y;
  for (x = 1; x <= s2len; x++) {
    column[0] = x;
    for (y = 1, lastdiag = x-1; y <= s1len; y++) {
      olddiag = column[y];
      column[y] = Minimum(column[y] + 1, column[y-1] + 1, lastdiag + (s1[y-1] == s2[x-1] ? 0 : 1));
      lastdiag = olddiag;
    }
  }
  return(column[s1len]);
}
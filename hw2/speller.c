#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "stringutil.h"
#include "hash.h"

char *zeroVal = "zero";
char *oneVal = "one";
char *twoVal = "two";

int calcDistance(char *word, char *dictionary, Hashmap *hashmap) {
  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  fp = fopen(dictionary, "r");
  if (fp == NULL)
    exit(1);
  while ((read = getline(&line, &len, fp)) != -1) {
    char *saveLine;

    trim(line);

    int ld = LevenshteinDistance(word, line);
    if(ld == 0) {
      hashmapSet(hashmap, line, zeroVal);
      return 1;
    }
    if(ld == 1) {
      saveLine = (char*)calloc(1,strlen(line));
      strncpy(saveLine, line, strlen(line));
      hashmapSet(hashmap, saveLine, oneVal);
    }
    if(ld == 2) {
      saveLine = (char*)calloc(1,strlen(line));
      strncpy(saveLine, line, strlen(line));
      hashmapSet(hashmap, saveLine, twoVal);
    }

  }
  if (ferror(fp)) {
        /* handle error */
  }
  free(line);
  fclose(fp);
  return 0;
}

int countWords(Hashmap *map, List *keyList, char *expected) {
  int count = 0;
  ListNode *currentKey = keyList->first;
  if(currentKey != NULL) {
    char *word = currentKey->value;
    char *value = hashmapGet(map, word);
    if(strcmp(value, expected) == 0) {
      count++;
    }
    while(currentKey->next != NULL) {
      currentKey = currentKey->next;
      char *word = currentKey->value;
      char *value = hashmapGet(map, word);
      if(strcmp(value, expected) == 0) {
        count++;
      }
    }
  }
  return count;
}

void printWords(Hashmap *map, List *keyList, char *expected, int count) {
  //printf("Count: %d\n", count);
    ListNode *currentKey = keyList->first;
    if(currentKey != NULL) {
      char *word = currentKey->value;
      char *value = hashmapGet(map, word);
      if(strcmp(value, expected) == 0 && count > 0) {
        printf("%s\n", word);
        count--;
      }
      while(currentKey->next != NULL && count > 0) {
        currentKey = currentKey->next;
        char *word = currentKey->value;
        char *value = hashmapGet(map, word);
        if(strcmp(value, expected) == 0) {
          printf("%s\n", word);
          count--;
        }
      }
      return;
    }
}

void checkWord(char *word, char *dictionary, int matchCount) {
  Hashmap *map = NULL;
  map = hashmapCreate(NULL, NULL);

  calcDistance(word, dictionary, map);

  List *keyList = hashmapGetKeys(map);
  int zeroCount = countWords(map, keyList, zeroVal);
  //printf("0 values: %d\n", zeroCount);
  if(zeroCount > 0) {
    printWords(map, keyList, zeroVal, 1);
  } else {
    int results1 = countWords(map, keyList, oneVal);
    //printf("1 values: %d\n", results);
    if(results1 >= matchCount) {
      printWords(map, keyList, oneVal, matchCount);
    } else {
      printWords(map, keyList, oneVal, results1);
      int remaining = matchCount - results1;
      //printf("%d remaining\n", remaining);
      if(remaining > 0) {
        int results2 = countWords(map, keyList, twoVal);
        //printf("2 values: %d\n", results);
        printWords(map, keyList, twoVal, remaining);
        if(results1 == 0 && results2 == 0) printf("\n");
      }
    }
  }

  listDestroy(keyList);
  hashmapDestroy(map);
  printf("<end>\n");

}

int main (int argc, char **argv) {
  int c;

  unsigned int matchCount = 12;
  char *dictionary = "/usr/share/dict/words";

  while (1) {
    static struct option long_options[] =
    {
           /* These options set a flag. */
     {"dictionary", required_argument, 0, 'd'},
     {"count",   required_argument, 0, 'n'},
     {"version", no_argument, 0, 'v'},
     {"help",  no_argument, 0, 'h'},
     {0, 0, 0, 0}
   };
   /* getopt_long stores the option index here. */
   int option_index = 0;

   c = getopt_long (argc, argv, "d:n:v:h",
    long_options, &option_index);

   /* Detect the end of the options. */
   if (c == -1)
    break;

  int y;

  switch (c) {
   case 0:
         /* If this option set a flag, do nothing else now. */
   if (long_options[option_index].flag != 0)
     break;

   break;

   case 'd':

   if( access( optarg, F_OK ) != -1 ) {
    dictionary = optarg;
   } else {
       printf("Could not open dictionary file.  Exiting.\n");
       return 0;
   }
   
   break;

   case 'n':
   for(y = 0; y < strlen(optarg); y++) {
      if(!isdigit(optarg[y])) {
      printf("value for -n is invalid.  Please try again.  Exiting.\n");
      return 0;
    }
   
   }
   sscanf(optarg, "%d", &matchCount);
   break;

   case 'v':
   printf ("My spelling checker, version 0.1\n");
   return 0;
   break;

   case 'h':
   printf ("My spelling checker, version 0.1\n");
   printf("To use, type in a word you want to have spell-checked.\n");
   printf("When you are done, type \"<end>\" or CTR-D\n");
   printf("Possible flags: \n");
   printf("-d, --dictionary    override default dictionary with another file\n");
   printf("-n, --count         override the default maximum number of suggestions\n"); 
   printf("-v, --version       print version information\n");
   printf("-h, --help          show help\n");
   return 0;
   break;

   case '?':
         /* getopt_long already printed an error message. */
   break;

   default:
   abort ();
 }
}

if (optind < argc) {
  printf("Command not recognized.  Exiting.\n");
  return 0;
}

char buffer[50];
char *end = "<end>";
while(fgets(buffer, sizeof(buffer)-1, stdin) != NULL) { 
  if (buffer[strlen(buffer)-1] != '\n') {
    printf("Input invalid.  Please try another input.\n");
    while ( getchar() != '\n' );
  } else {
    trim(buffer);
    if(strcmp(buffer, end) == 0) break;
    int buffLen = strlen(buffer);
    int i = 0;
    while(i < buffLen) {
      if(!isalpha(buffer[i])) {
        printf("Input invalid.  Please try another input.\n");
        break;
      } 
      i++;
    }
    checkWord(buffer, dictionary, matchCount);
  }
}

exit (0);
}
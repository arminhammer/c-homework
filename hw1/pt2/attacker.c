#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 256

#define NELEMS(x) ((sizeof (x))/(sizeof ((x)[0])))

// attack target
char *prog1 = "./fib";
char *prog2 = "./ifib";
char *prog3 = "./rfib";

int main ( int argc, char *argv[] )
{
   int i;
   int retval;

   char cmd[MAX];

   char *args[] = { "arg",
                    "a longer argument line for testing",
                    "nice crash",
                    "-",
                    "%s",
                    // Caution: in "system" call, command is interpreted by the shell. 
                    "spooky ; ls /",
                    "20f",
                    "e34",
                    "-help",
                    "-target 4",
                    "31",
                    "200",
                    "--t 4",
                    "-t 45"
                  };

   for ( i = 0; i < NELEMS(args); i++ ) {
      // construct command to be executed
      snprintf(cmd, sizeof(cmd), "%s %s", prog1, args[i]);

      fprintf(stdout, "\nExecuting: %s\n\n", cmd);

      /* 
       * DO NOT use "system" when input is not trusted. It can be disastrous. 
       * Use exec family of functions, NOT INCLUDING execlp and execvp. 
       */
      retval = system(cmd);

      fprintf(stdout, "\n\nReturn value: %d (%s)\n", retval, strerror(retval));
   }

   for ( i = 0; i < NELEMS(args); i++ ) {
      // construct command to be executed
      snprintf(cmd, sizeof(cmd), "%s %s", prog2, args[i]);

      fprintf(stdout, "\nExecuting: %s\n\n", cmd);

      /* 
       * DO NOT use "system" when input is not trusted. It can be disastrous. 
       * Use exec family of functions, NOT INCLUDING execlp and execvp. 
       */
      retval = system(cmd);

      fprintf(stdout, "\n\nReturn value: %d (%s)\n", retval, strerror(retval));
   }

   for ( i = 0; i < NELEMS(args); i++ ) {
      // construct command to be executed
      snprintf(cmd, sizeof(cmd), "%s %s", prog3, args[i]);

      fprintf(stdout, "\nExecuting: %s\n\n", cmd);

      /* 
       * DO NOT use "system" when input is not trusted. It can be disastrous. 
       * Use exec family of functions, NOT INCLUDING execlp and execvp. 
       */
      retval = system(cmd);

      fprintf(stdout, "\n\nReturn value: %d (%s)\n", retval, strerror(retval));
   }

   return 0;
}
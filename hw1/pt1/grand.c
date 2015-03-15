#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* Generates a random integer. */
void getRandom(int count) {

	int i;
	for(i=0; i < count; i++) {
		printf("%d ", rand() % 101);
	}
	
	printf("\n");
}

/* Prints a helpful message on how to use the program. */
void printHelp() {
	printf("\n  grand [-h | --help]       : output this usage message.\n");
	printf("  grand [-v | --version]    : print the program version number.\n");
	printf("  grand [n]                 : print out n random integers in [0,100]\n\n");
}

/* If the program cannot parse the input, print a message. */
void printParseError() {
	printf("\n  Could not parse input.  Please use the following guide to the program:\n");
	printHelp();
}

int main(int argc, char *argv[])
{
	// Get a random seed
	srand ( time(NULL) );

	// If there are no arguments, print 10 random integers
	if(argc == 1) {
		getRandom(10);
	} 
	// First check to see if we can parse the strings.  If not, try to parse for an int.
	else if(argc == 2) {
		if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
			printHelp();
			return 0;
		}
		else if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)
		{
			printf("\n  grand-0.0.3\n\n");
		}
		else {
			// Try to parse for an int.  If not, throw an error.
			int errno = 0;
			char *ptr = argv[1];
			unsigned int val = strtoul(argv[1], &ptr, 10);
			if (errno != 0) {
    			// conversion failed (EINVAL, ERANGE)
				printf("Error: %d\n", errno);
				printParseError();
				return 0;
			}
			if (argv[1] == ptr) {
    			// conversion failed (no characters consumed)
				printParseError();
				return 0;
			}
			if (*ptr != 0) {
    			// conversion failed (trailing data)
				printParseError();
				return 0;
			}
			getRandom(val);
		}
	}
	else {
		printParseError();
		return 0;
	}

	return 0;
}

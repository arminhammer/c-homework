#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Recursive Fibannaci algorithm */
unsigned int fibRecursive(unsigned int target)
{
	if(target == 0) {
		return 1;
	} 
	if (target == 1) {
		return 1;
	}
 	fibRecursive(target - 1);
 	unsigned int result = (fibRecursive(target - 1) + fibRecursive(target - 2));
 	return result;
}

/* Iterative Fibannaci algorithm */
unsigned int fibIterative(unsigned int n) {
	
	if(n == 0) {
		return 1;
	}
	if(n == 1) {
		return 1;
	}

	int prevPrev = 1;
	int prev = 1;
	unsigned int result = 0;
	int i;
	for (i = 2; i <= n; i++ )
	{
		result = prev + prevPrev;
		prevPrev = prev;
		prev = result;
	}
	return result;
} 

/* Print message if the input cannot be parsed. */
void printParseError() {
	printf("Could not parse input.\n");
}

int main(int argc, char *argv[])
{
	// Initialize the errno variable for input validation
	int errno = 0;

	// Test to see whether the recursive or iterative algorithm should be used
	int useRecursive = 0;
	if(strcmp(argv[0], "./fib") == 0 || strcmp(argv[0], "./rfib") == 0) {
		useRecursive = 1;
	}

	// If there is one argument, test to see if it is an integer
	if(argc == 2) {
		char *ptr = argv[1];
		unsigned int inputVal = strtoul(argv[1], &ptr, 10);
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
		if (inputVal > 30) {
			printParseError();
			return 0;
		}
		int i;
		if(useRecursive == 1) {
			for(i = 0; i < inputVal+1; i++) {
				printf("fib(%d) = %d\n", i, fibRecursive(i));
			}
		}
		else {
			for(i = 0; i < inputVal+1; i++) {
				printf("fib(%d) = %d\n", i, fibRecursive(i));
			}
		}
	} 
	// If there are two arguments, check to see if it is -t or --target [n]
	else if(argc == 3) {
		if(strcmp(argv[1], "-t") == 0 || strcmp(argv[1], "--target") == 0) {
			char *ptr = argv[2];
			unsigned int inputVal = strtoul(argv[2], &ptr, 10);
			if (errno != 0) {
				// conversion failed (EINVAL, ERANGE)
				printf("Error: %d\n", errno);
				printParseError();
				return 0;
			}
			if (argv[2] == ptr) {
				// conversion failed (no characters consumed)
				printParseError();
				return 0;
			}
			if (*ptr != 0) {
				// conversion failed (trailing data)
				printParseError();
				return 0;
			}
			if (inputVal > 30) {
				printParseError();
				return 0;
			}
			if(useRecursive == 1) {
				printf("fib(%d) = %d\n", inputVal, fibRecursive(inputVal));
			}
			else {
				printf("fib(%d) = %d\n", inputVal, fibIterative(inputVal));
			}
		}
		else {
			printParseError();
			return 0;
		}
	}
	else {
		printParseError();
		return 0;
	}
	return 0;
}

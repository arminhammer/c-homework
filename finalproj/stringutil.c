#include "stringutil.h"

//From http://en.wikipedia.org/wiki/Trimming_(computer_programming)
void rtrim(char *str) {
	size_t n;
	n = strlen(str);
	while (n > 0 && isspace((unsigned char )str[n - 1])) {
		n--;
	}
	str[n] = '\0';
}

//From http://en.wikipedia.org/wiki/Trimming_(computer_programming)
void ltrim(char *str) {
	size_t n;
	n = 0;
	while (str[n] != '\0' && isspace((unsigned char )str[n])) {
		n++;
	}
	memmove(str, str + n, strlen(str) - n + 1);
}

//From http://en.wikipedia.org/wiki/Trimming_(computer_programming)
void trim(char *str) {
	rtrim(str);
	ltrim(str);
}

/*
 * Remove the first character in a string
 */
void chopFront(char *string) {
	if (string[0] != '\0') {
		memmove(string, string + 1, strlen(string));
	}
}

/*
 * Remove n characters from the front of a string
 */
void chopFrontCount(char *string, int count) {
	int i;
	for (i = 0; i < count; i++) {
		chopFront(string);
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
	for (i = 0; i < strlen(string); i++) {
		if (string[i] == letter)
			count++;
	}
	return count;
}

/*
 * Check if a string starts with a string value
 */
int startsWith(char *input, char *target) {
	int len = strlen(target);
	int i;
	for (i = 0; i < len; i++) {
		if (target[i] != input[i]) {
			return -1;
		}
	}
	return 0;
}

/*
 * See if a string ends with a value
 */
int endsWith(char *ending, char *target) {
	int targetLen = strlen(target);
	int endingLen = strlen(ending);
	if (endingLen > targetLen)
		return -1;
	while (endingLen > 0) {
		if (ending[endingLen-1] != target[targetLen-1]) {
			return -1;
		} else {
			endingLen--;
			targetLen--;
		}
	}
	return 0;
}

/*
 * Read a string into a buffer until a character is found
 */
int readUntilChar(char *buffer, char *input, char letter) {

	char *buff = calloc(1, strlen(input) + 1);
	int i = 0;

	while (input[0] != '\0') {
		if (input[0] != letter) {
			buff[i] = input[0];
			++i;
			chopFront(input);
		} else {
			strcpy(buffer, buff);
			free(buff);
			chopFront(input);
			return 0;
		}
	}
	free(buff);
	return -1;

}

/*
 * Write a line into a buffer until \n is hit
 */
int getlineFromString(char *buffer, char *input) {

	char *buff = NULL;
	buff = calloc(1, strlen(input) + 1);

	int i = 0;

	while (input[0] != '\0') {
		if (input[0] != '\n') {
			buff[i] = input[0];
			i++;
			chopFront(input);
		} else {
			strcpy(buffer, buff);
			chopFront(input);
			free(buff);
			return 0;
		}
	}
	free(buff);
	return -1;

}

/*
 * Return a current datestamp
 */
char *getDateStamp() {
	time_t ltime; /* calendar time */
	ltime = time(NULL); /* get current cal time */
	char *time = asctime(localtime(&ltime));
	time[strlen(time)-1] = '\0';
	return time;
}

/*
 * Get the current directory
 */
char *getCurrentDir() {
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		char *dir = calloc(1, sizeof(cwd));
		strncpy(dir, cwd, sizeof(cwd));
		return dir;
	}
	return NULL;
}


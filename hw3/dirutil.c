/*
 * dirutil.c
 *
 *  Created on: Apr 8, 2014
 *      Author: armin
 */
#include "dirutil.h"

/*
 * Get the current directory
 */
char *getCurrentDir() {
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		//printf("Current working dir: %s\n", cwd);
		char *dir = calloc(1, sizeof(cwd));
		strncpy(dir, cwd, sizeof(cwd));
		return dir;
	}
	return NULL;
}

/*
 * Get the parent directory of the current directory
 */
char *getParentDir(char *dirString) {
	char *returnString = strdup(dirString);
	//strncpy(returnString, dirString);
	if (strcmp(returnString, "/") == 0)
		return returnString;

	if (returnString[strlen(returnString) - 1] == '/') {
		returnString[strlen(returnString) - 1] = '/';
	}

	size_t n;
	n = strlen(returnString);
	while (n > 0 && returnString[n - 1] != '/') {
		n--;
	}
	if (n == 1) {
		returnString[n] = '\0';
	} else {
		returnString[n - 1] = '\0';
	}
	return returnString;
}

/*
 * Change the current directory
 */
void changeDir(char* targetDir) {
	int change = chdir(targetDir);
	if (change == -1) {
		printf("Unable to change directory to %s\n", targetDir);
	}
	return;
}


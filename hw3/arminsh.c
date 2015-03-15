#include <ctype.h>

#include "stringutil.h"
#include "stack.h"
#include "dirutil.h"
#include "forkutil.h"

int historySize = 100;

Stack *history;
Stack *dirStack;

char *previousDir;
char *currentDir;

/*
 * Try to process the command.  First check if it is obvious.  If not, separate
 * the command from the arguments and try to execute it that way
 */
int execCommand(char *buffer) {
	char *newCommand = calloc(1, sizeof(buffer));
	strcpy(newCommand, buffer);

	if (strcmp(buffer, "exit") == 0 || strcmp(buffer, "quit") == 0) {
		printf("Exiting arminsh.\n");
		free(newCommand);
		return 1;
	} else if (strcmp(buffer, "pwd") == 0) {
		printf("%s\n", currentDir);
	} else if (strcmp(buffer, "history") == 0) {
		printf("history:\n");
		printStack(history);
	} else {

		//char* token;
		char* string;
		char* toFree;

		string = strdup(newCommand);
		//int charSize = sizeof(newCommand);

		if (string != NULL) {

			toFree = string;

			char *commandString = strsep(&string, " ");
			char *argString = NULL;
			if (string != NULL) {
				argString = strdup(string);
			}
			//printf("Command is: %s\n", commandString);
			//printf("args are: %s\n", argString);
			if (commandString != NULL)
				trim(commandString);
			if (argString != NULL)
				trim(argString);

			if (strcmp(commandString, "cd") == 0) {
				//printf("You selected to ch into %s\n", argString);
				if (strcmp(argString, ".") == 0) {
					//printf("not changing dirs.\n");
				} else if (strcmp(argString, "..") == 0) {
					//printf("changing to parent dir.\n");
					char *parentDir = getParentDir(currentDir);
					//printf("Parent dir of %s is %s\n", currentDir, parentDir);
					changeDir(parentDir);
					previousDir = strdup(currentDir);
					free(currentDir);
					currentDir = getCurrentDir();
					free(parentDir);
				} else if (strcmp(argString, "-") == 0) {
					//printf("changing to previous dir.\n");
					if (previousDir != NULL) {
						changeDir(previousDir);
						free(previousDir);
						previousDir = strdup(currentDir);
						free(currentDir);
						currentDir = getCurrentDir();
					}
				} else {
					//printf("changing to %s.\n", argString);
					changeDir(argString);
					free(previousDir);
					previousDir = strdup(currentDir);
					free(currentDir);
					currentDir = getCurrentDir();
				}
			} else if (strcmp(commandString, "pushd") == 0) {
				char *pushDir = strdup(currentDir);
				push(dirStack, pushDir);
			} else if (strcmp(commandString, "popd") == 0) {
				char *poppedDir = pop(dirStack);
				changeDir(poppedDir);
				currentDir = getCurrentDir();
				free(poppedDir);
			} else if (strcmp(commandString, "dirs") == 0) {
				printStack(dirStack);
			} else if (commandString[0] == '!' && argString == NULL) {
				if (strlen(commandString) < 5) {
					//printf("Executing %s!\n", commandString);
					//size_t cLength = sizeof(commandString);
					int i = 1;
					int isNumber = 0;
					while (i < strlen(commandString)) {
						if (isdigit(commandString[i]) == 0) {
							printf("%c is not a number!\n", commandString[i]);
							isNumber = 1;
							break;
						}
						i++;
					}
					if (isNumber == 0) {
						//printf("command before: %s\n", commandString);
						chopFront(commandString);
						//printf("command after: %s\n", commandString);
						unsigned int num = parseInt(commandString);
						if (num < historySize + 1) {
							//printf("Print command %d\n", num);
							char *historyCommand = peek(history, num - 1);
							//printf("Fork command %s\n", historyCommand);
							execCommand(historyCommand);
						}
					}
				}

			} else {
				int tryExec;
				if (argString != NULL) {
					int argLength = countHits(argString, ' ') + 3;

					char *argArray[argLength];

					argArray[0] = commandString;
					//argArray[1] = NULL;
					//argArray[0] = argString;

					int i;
					for (i = 1; i < argLength; i++) {
						argArray[i] = strsep(&argString, " ");
					}

					tryExec = execute(commandString, argArray, currentDir);
				} else {
					tryExec = execute(commandString, NULL, currentDir);
				}
				if(tryExec != 0) {
					printf("Could not execute command.  Error message: %d\n", tryExec);
				}
			}
			free(toFree);
			free(argString);
		}

	}
	//printf("Input: %s\n", buffer);
	char *newCommandRecord = strdup(newCommand);
	push(history, newCommandRecord);
	free(newCommand);
	return 0;
}

int main(int argc, char **argv) {

	if (argc > 1) {
		printf("arminsh does not accept flags.\n");
		return 0;
	}

	// Stack for history commands
	history = createStack(historySize);
	// Stack for pushd/popd/dirs
	dirStack = createStack();

	// keep track of the current and previous dirs
	previousDir = NULL;
	currentDir = getCurrentDir();

	// Limit the input buffer to a reasonable max
	char buffer[150];

	//print the shell prompt
	printf("%s $ ", currentDir);

	//process input
	while (fgets(buffer, sizeof(buffer) - 1, stdin) != NULL) {

		if (buffer[strlen(buffer) - 1] != '\n') {
			printf("Input invalid.  Please try another input.\n");
			while (getchar() != '\n')
				;
			goto newLine;
		} else {
			trim(buffer);
		}

		// try to execute the command
		int execResult = execCommand(buffer);
		if(execResult == 1) { break; }
		newLine: printf("%s $ ", currentDir);
	}

	// clean up
	destroyStack(history);
	destroyStack(dirStack);
	free(previousDir);
	free(currentDir);
	return 0;
}

/*
 * forkutil.c
 *
 *  Created on: Apr 8, 2014
 *      Author: armin
 */
#include "forkutil.h"

char *path[4] = { "/bin", "/usr/bin", "/sbin", "/usr/sbin" };

/*
 * fork() the process
 */
void forkXecute(char *command, char *args[]) {
	pid_t child_pid; /* i.e., int  pid; */
	int status;
	pid_t wait_result;

	child_pid = fork();

	if (child_pid == 0) {
		/* this code is only executed in the child process */
		//printf("I am a child and my pid = %d\n", getpid());
		if (args == NULL) {
			execl(command, command, NULL);
		} else {
			execv(command, args);
		}
		/* if execl succeeds, this code is never used */
		//printf("Could not execv file %s\n", command);
		exit(1);
		/* this exit stops only the child process */
	} else if (child_pid > 0) {
		/* this code is only executed in the parent process */
		//printf("I am the parent and my pid %d\n", getpid());
		//printf("My child has pid = %d\n", child_pid);
	} else {
		//printf("The fork system call failed to create a new process\n");
		exit(1);
	}

	/* this code is executed by only the parent process because
	 the child either is executing from "/bin/ls" or exited.  */
	//printf("I am a happy, healthy process and my pid = %d\n", getpid());

	if (child_pid == 0) {
		/* this code will never be executed */
		//printf("This code will never be executed!\n");
	} else {
		/* this code is only executed by the parent process */
		//printf("I am a parent and I am going to wait for my child\n");
		do {
			/* parent waits for the SIGCHLD signal sent
			 to the parent of a (child) process when
			 the child process terminates */
			wait_result = wait(&status);
		} while (wait_result != child_pid);
		//printf("I am a parent and I am quitting.\n");
	}
	return;

}

/*
 * Figure out where the executable is, and then try to fork()
 */
int execute(char *command, char *args[], char *currentDir) {

	char toExecute[1024];
	char *execDirPath = strdup(currentDir);
	char exeInCurrentPath[strlen(currentDir) + strlen(command) + 1];
	//printf("strlen currentDir is %lu, strlen command is %lu, strlen / is %lu\n", strlen(currentDir), strlen(command), strlen("/"));
	strcpy(exeInCurrentPath, currentDir);
	strcat(exeInCurrentPath, "/");
	strcat(exeInCurrentPath, command);

	if (command[0] == '.' && command[1] == '/') {
		chopFront(command);
		char *execDirPath2 = strdup(execDirPath);
		strcpy(toExecute, execDirPath2);
		strcat(toExecute, command);
	} else if (access(command, F_OK) != -1) {
		strcpy(toExecute, command);
	} else if (access(exeInCurrentPath, F_OK) != -1) {
		strcpy(toExecute, execDirPath);
		strcat(toExecute, command);
	} else {
		int i;
		int found = 0;
		for (i = 0; i < sizeof(path); i++) {
			if (found != 0)
				break;
			char *currentPath = path[i];
			//printf("Looking into %s\n", currentPath);
			DIR *dir;
			struct dirent *ent;
			if ((dir = opendir(currentPath)) != NULL) {
				/* print all the files and directories within directory */
				while ((ent = readdir(dir)) != NULL) {
					//printf("%s\n", ent->d_name);
					if (strcmp(command, ent->d_name) == 0) {
						//printf("%s is in %s\n", command, currentPath);
						found = 1;
						//toExecute = calloc(1, (strlen(currentPath) + strlen(command) + 1));
						strcpy(toExecute, currentPath);
						strcat(toExecute, "/");
						strcat(toExecute, command);
						break;
					}
				}
				closedir(dir);
			} else {
				/* could not open directory */
				perror("");
				//free(toExecute);
				//printf("Free1\n");
				free(execDirPath);
				//printf("Free2\n");
				free(exeInCurrentPath);
				//printf("Free3\n");
				return EXIT_FAILURE;
			}
		}
	}
	//printf("toExecute is: %s\n", toExecute);
	if (toExecute == NULL) {
		//free(toExecute);
		free(execDirPath);
		free(exeInCurrentPath);
		return 1;
	}

	forkXecute(toExecute, args);

	//free(toExecute);
	free(execDirPath);
	//free(exeInCurrentPath);
	return 0;

}


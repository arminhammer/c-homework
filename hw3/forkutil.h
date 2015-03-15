/*
 * forkutil.h
 *
 *  Created on: Apr 8, 2014
 *      Author: armin
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>

#include "stringutil.h"

#ifndef FORKUTIL_H_
#define FORKUTIL_H_

void forkXecute(char *command, char *args[]);
int execute(char *command, char *args[], char *currentDir);

#endif /* FORKUTIL_H_ */

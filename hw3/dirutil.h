/*
 * dirutil.h
 *
 *  Created on: Apr 8, 2014
 *      Author: armin
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifndef DIRUTIL_H_
#define DIRUTIL_H_

char *getCurrentDir();
char *getParentDir(char *dirString);
void changeDir(char* targetDir);

#endif /* DIRUTIL_H_ */

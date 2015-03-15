/*
 * config.h
 *
 *  Created on: Apr 20, 2014
 *      Author: armin
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stringutil.h"

#ifndef CONFIG_H_
#define CONFIG_H_

/*
 * Object to store server configuration information
 */
typedef struct Config {
	char *rootdir;
	char *moddir;
	int port;
	char *logfile;
	char *config;
} Config;

Config *buildConfig();
int readConfigFile(char *path, Config *config);

#endif /* CONFIG_H_ */

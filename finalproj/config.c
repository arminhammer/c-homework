/*
 * config.c
 *
 *  Created on: Apr 20, 2014
 *      Author: armin
 */

#include "config.h"

/*
 * Construct the config object
 */
Config *buildConfig() {
	Config *config = calloc(1, sizeof(Config));
	return config;
}

/*
 * Extract the value from a line in the file
 */
int extractValue(char *buffer, char *line) {

	char *lineBuffer = calloc(1, strlen(line));

	int i = 0;
	int y = 0;
	while (i < strlen(line)) {
		if (line[i] != '"')
			i++;
		else {
			i++;
			while (line[i] != '"') {
				lineBuffer[y] = line[i];
				i++;
				y++;
			}
			strcpy(buffer, lineBuffer);
			free(lineBuffer);
			return 0;
		}
	}

	free(lineBuffer);
	return -1;

}

/*
 * Main function to parse the file.
 */
int readConfigFile(char *path, Config *config) {
	FILE *fp;
	char *line = NULL;
	//line[0] = 0;
	size_t len = 0;
	ssize_t read = 0;
	fp = fopen(path, "r");
	if (fp == NULL)
		exit(1);
	while ((read = getline(&line, &len, fp)) != -1) {

		if (startsWith(line, "rootdir=\"") == 0) {
			char *rootDirLine = calloc(1, strlen(line) + 1);
			extractValue(rootDirLine, line);
			config->rootdir = rootDirLine;
		} else if (startsWith(line, "moddir=\"") == 0) {
			char *modDirLine = calloc(1, strlen(line) + 1);
			extractValue(modDirLine, line);
			config->moddir = modDirLine;
		} else if (startsWith(line, "logfile=\"") == 0) {
			char *logLine = calloc(1, strlen(line) + 1);
			extractValue(logLine, line);
			config->logfile = logLine;
		} else if (startsWith(line, "port=\"") == 0) {
			char *portLine = calloc(1, strlen(line) + 1);
			extractValue(portLine, line);
			config->port = parseInt(portLine);
			free(portLine);
		}

		printf("Line is %s\n", line);

	}
	if (ferror(fp)) {
		/* handle error */
		return -1;
	}
	free(line);
	fclose(fp);
	return 0;
}


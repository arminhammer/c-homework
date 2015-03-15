/*
 * ihttpd.c
 *
 *  Created on: Apr 13, 2014
 *      Author: armin
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <dlfcn.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <errno.h>

#include "stringutil.h"
#include "http.h"
#include "config.h"

Config *config;

// This should only be used with -c, change...
char *configFilePath = "config.cfg";

/* function prototypes and global variables */
void * processRequest(void * ptr);
void writeLog(char *client_ip, char *method, char *resource, char *status,
		int bytesSent);

typedef struct {
	int sock;
	struct sockaddr address;
	socklen_t addr_len;
} connection_t;

// Define the module method signature
typedef char *def_mod(char *arg);
def_mod *modMethod;

/*
 * Function to check if the file system resource exists
 */
int verifyResource(char *resource) {

	if (access(resource, F_OK) != -1) {
		// file exists
		struct stat buffer;
		if (stat(resource, &buffer) == 0 && S_ISDIR(buffer.st_mode)) {
			return 2;
		} else {
			return 1;
		}
	} else {
		//file doesn't exist
		return -1;
	}

}

/*
 * Function to handle dynamic module loading if one is detected in the URL
 */
int handleModule(char *output, char *resourceString) {

	char *buffer = calloc(1, strlen(resourceString) + 1);
	char *modName = calloc(1, strlen(resourceString) + 1);
	strncpy(buffer, resourceString, strlen(resourceString));
	chopFrontCount(buffer, 5);
	readUntilChar(modName, buffer, '?');
	char *argString;
	if (strlen(buffer) > 0) {
		argString = calloc(1, strlen(buffer) + 1);
		strncpy(argString, buffer, strlen(buffer));
	} else {
		argString = "";
	}
	printf("The mod name is %s\n", modName);
	char *fullModPath;
	int isAbsolute = verifyResource(config->moddir);
	if (isAbsolute == 2) {
		fullModPath = calloc(1, strlen(config->moddir) + strlen(modName) + 10);
		strcpy(fullModPath, config->moddir);
		if (endsWith("/", fullModPath) != 0) {
			strcat(fullModPath, "/");
		}
	} else {
		fullModPath = calloc(1, strlen(config->moddir) + strlen(modName) + 10);
		if (endsWith("/", fullModPath) == 0) {
			strcpy(fullModPath, config->moddir);
		} else {
			strcpy(fullModPath, "/");
			strcat(fullModPath, config->moddir);
		}
		isAbsolute = verifyResource(fullModPath);
		if (isAbsolute != 2) {
			printf("Mod could not be found!\n");
			free(buffer);
			free(modName);
			free(fullModPath);
			return -1;
		}
	}

	strcat(fullModPath, modName);
	strcat(fullModPath, ".so");
	int exists = verifyResource(fullModPath);

	if (exists != 1) {
		printf("Mod does not exist!\n");
		free(buffer);
		free(modName);
		free(fullModPath);
		return -1;
	}

	void *module = dlopen(fullModPath, RTLD_LAZY);
	if (module == NULL) {
		char *error = dlerror();
		printf("Could not load module %s, error: %s\n", fullModPath, error);
		free(buffer);
		free(modName);
		free(fullModPath);
		return -1;
	}

	modMethod = dlsym(module, "def_mod");
	char *result = dlerror();
	if (result) {
		printf("Cannot find init in %s: %s\n", modName, result);
	}
	char *content = modMethod(argString);
	char *content1 = calloc(1, strlen(content) + 4);
	strcpy(content1, content);
	strcat(content1, "\r\n");
	strncpy(output, content1, strlen(content1));
	free(buffer);
	free(modName);
	free(content);
	free(content1);
	free(fullModPath);
	free(argString);
	return 0;

}

/*
 * Function to get a lock on the log file and write the log message.
 */
void writeLog(char *client_ip, char *method, char *resource, char *status,
		int bytesSent) {

	printf("The time is %s\n", getDateStamp());

	FILE * log;
	int error, timeout;
	log = fopen(config->logfile, "a");

	error = lockf(fileno(log), F_TLOCK, 0);
	while (error == EACCES || error == EAGAIN) {
		//sleep for a bit
		usleep(1000);

		//Incremement timeout
		timeout += 1000;

		//Check for time out
		if (timeout > 30000) {
			return;
		}

		//Retry the lock operation
		error = lockf(fileno(log), F_TLOCK, 0);
	}

	//Print log entry
	fseek(log, 0, SEEK_END);
	char *timeStamp = calloc(1, 200);
	strcpy(timeStamp, getDateStamp());
	trim(timeStamp);
	timeStamp[strlen(timeStamp) - 1] = 0;
	fprintf(log, "%s [%s] %s %s %s %d\n", client_ip, timeStamp, method,
			resource, status, bytesSent);
	fflush(log);
	//Unlock the block
	//rewind(start);
	lockf(fileno(log), F_ULOCK, 0);

	fclose(log);

	free(timeStamp);
	return;

}

/*
 * Get the file content of a text file
 */
void getTextContent(char* fullResourcePath, HTTPResponse* response) {

	FILE* fp;
	fp = fopen(fullResourcePath, "r");
	fseek(fp, 0L, SEEK_END);
	unsigned long fileLen = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	printf("Filesize is %lu\n", fileLen);
	char *buffer = calloc(1, (fileLen + 1) * sizeof(char));

	if (buffer != NULL) {
		struct stat sb;
		stat(fullResourcePath, &sb);
		response->lastModified = calloc(1, strlen(ctime(&sb.st_mtime)) + 1);
		strcpy(response->lastModified, ctime(&sb.st_mtime));
		trim(response->lastModified);
		long bytesRead = fread(buffer, fileLen, 1, fp);
		printf("%lu bytes read\n", bytesRead);
		// we can now close the file
		fclose(fp);
		fp = NULL;

		// do something, e.g.
		response->status = "200";
		response->statusString = "OK";
		response->content = NULL;
		response->content = calloc(1, strlen(buffer) + 1);
		strcpy(response->content, buffer);
		response->contentLength = fileLen;

	}
	free(buffer);
	if (fp != NULL)
		fclose(fp);
}

/*
 * Get the file contents of a binary file
 */
void getBinaryContent(char* fullResourcePath, HTTPResponse* response) {

	FILE* fp;
	fp = fopen(fullResourcePath, "rb");

	fseek(fp, 0L, SEEK_END);
	long fileLen = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	printf("Filesize is %lu\n", fileLen);

	char *buffer = (char*) calloc(1, (fileLen + 1) * sizeof(char*));
	printf("buffer is %lu big, and len %lu\n", sizeof(buffer), strlen(buffer));
	if (buffer != NULL) {
		struct stat sb;
		stat(fullResourcePath, &sb);
		response->lastModified = calloc(1, strlen(ctime(&sb.st_mtime)) + 1);
		strcpy(response->lastModified, ctime(&sb.st_mtime));
		trim(response->lastModified);

		unsigned long bytesRead = fread(buffer, 1, fileLen, fp);
		printf("%lu bytes read\n", bytesRead);
		// we can now close the file
		fclose(fp);
		printf("buffer is now %lu big, and len %lu\n", sizeof(&buffer),
				strlen(buffer));
		fp = NULL;

		// do something, e.g.
		response->status = "200";
		response->statusString = "OK";
		response->content = calloc(1, fileLen * sizeof(char));
		memcpy(response->content, buffer, fileLen);
		response->contentLength = fileLen;

	}
	free(buffer);
	if (fp != NULL)
		fclose(fp);
}

/*
 * This will handle connection for each client
 * */
void *processRequest(void *socket_desc) {

	//Get the socket descriptor
	int sock = *(int*) socket_desc;
	int read_size;
	//char *message,
	char client_message[1024];

	read_size = recv(sock, client_message, sizeof(client_message), 0);

	if (read_size < 0) {
		perror("Error reading from socket, exiting thread\n");
		pthread_exit(0);
	}

	//char *requestMessage = client_message;
	printf("Parsing request...\n");
	printf("client_message before trim: \n%s\n", client_message);
	trim(client_message);
	printf("client_message before trim: \n%s\n", client_message);
	HTTPRequest *request = parseRequest(client_message);
	printf("Done parsing request.\n");

	HTTPResponse *response = buildResponse();

	response->protocol = "HTTP/1.0";

	if (request->resource != NULL) {
		if (startsWith(request->resource, "/mod/") == 0) {
			printf("Request started with /mod/!\n");
			response->content = calloc(1, strlen(request->resource) + 100);

			int modResult = handleModule(response->content, request->resource);
			if (modResult == 0) {
				response->status = "200";
				response->statusString = "OK";
				response->contentType = "text/html";
				response->contentLength = strlen(response->content);
				response->lastModified = calloc(1, 200);
				strcpy(response->lastModified, getDateStamp());
			} else {
				generate404(response);
			}
		} else {
			char *fullResourcePath = calloc(1,
					strlen(config->rootdir) + strlen(request->resource) + 1);
			strcpy(fullResourcePath, config->rootdir);
			strcat(fullResourcePath, request->resource);

			int exists = verifyResource(fullResourcePath);

			if (exists == 1) {

				if (endsWith(".html", fullResourcePath) == 0
						|| endsWith(".htm", fullResourcePath) == 0) {
					response->contentType = "text/html";
					getTextContent(fullResourcePath, response);
				} else if (endsWith(".txt", fullResourcePath) == 0) {
					response->contentType = "text/plain";
					getTextContent(fullResourcePath, response);
				} else if (endsWith(".jpg", fullResourcePath) == 0
						|| endsWith(".jpeg", fullResourcePath) == 0) {
					response->contentType = "image/jpeg";
					getBinaryContent(fullResourcePath, response);
				} else if (endsWith(".gif", fullResourcePath) == 0) {
					response->contentType = "image/gif";
					getBinaryContent(fullResourcePath, response);
				} else if (endsWith(".png", fullResourcePath) == 0) {
					response->contentType = "image/png";
					getBinaryContent(fullResourcePath, response);
				}

			} else if (exists == 2) {
				printf("The resource is a folder!\n");

				char *dirIndex = calloc(1, strlen(fullResourcePath) + 15);
				strcpy(dirIndex, fullResourcePath);
				if (endsWith("/", fullResourcePath) == 0) {
					strcat(dirIndex, "index.html");
				} else {
					strcat(dirIndex, "/index.html");
				}
				exists = verifyResource(dirIndex);
				if (exists == 1) {
					response->contentType = "text/html";
					getTextContent(dirIndex, response);
					free(dirIndex);
				} else {
					free(dirIndex);
					dirIndex = calloc(1, strlen(fullResourcePath) + 15);
					strcpy(dirIndex, fullResourcePath);
					if (endsWith("/", fullResourcePath) == 0) {
						strcat(dirIndex, "index.htm");
					} else {
						strcat(dirIndex, "/index.htm");
					}
					exists = verifyResource(dirIndex);
					if (exists == 1) {
						response->contentType = "text/html";
						getTextContent(dirIndex, response);
						free(dirIndex);
					} else {
						free(dirIndex);
						generate404(response);
					}
				}
			} else {
				generate404(response);
			}
			free(fullResourcePath);
		}
	} else {
		generate404(response);
	}

	socklen_t len;
	struct sockaddr_storage addr;
	char ipstr[INET_ADDRSTRLEN];

	len = sizeof addr;
	getpeername(sock, (struct sockaddr*) &addr, &len);

	struct sockaddr_in *s = (struct sockaddr_in *) &addr;

	inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
	char *methodString;
	int isGetRequest = 0;
	if (request->method == GET) {
		methodString = "GET";
		isGetRequest = 1;
	} else if (request->method == HEAD) {
		methodString = "HEAD";
	} else {
		printf("There was an issue determining the method.  Exiting.\n");
		return NULL;
	}
	int rSize = responseSize(response);
	writeLog(ipstr, methodString, request->resource, response->status, rSize);
	char *resString = calloc(1, rSize + 1);
	responseString(resString, response, isGetRequest);

	printf("Client message:\n\n %s\n\n", client_message);
	printf("Response:\n\n %s\n\n", resString);
	read_size = write(sock, resString, strlen(resString));

	if (read_size < 0) {
		perror("Error writing to socket, exiting thread");
		pthread_exit(0);
	}

	if (read_size == 0) {
		puts("Client disconnected");
		fflush(stdout);
	} else if (read_size == -1) {
		perror("recv failed");
	}
	if (isGetRequest == 1) {

		read_size = write(sock, response->content, response->contentLength);

		if (read_size < 0) {
			perror("Error writing to socket, exiting thread");
			pthread_exit(0);
		}

		if (read_size == 0) {
			puts("Client disconnected");
			fflush(stdout);
		} else if (read_size == -1) {
			perror("recv failed");
		}
	}

	destroyRequest(request);
	destroyResponse(response);

	free(resString);
	//Free the socket pointer
	close(sock);
	free(socket_desc);
	printf("Closing socket.\n");
	pthread_exit(0);

}

/*
 * The main function
 */
int main(int argc, char **argv) {

	/* Our process ID and Session ID */
	pid_t pid;
	/* Fork off the parent process */

	pid = fork();
	if (pid < 0) {
		exit(EXIT_FAILURE);
	}

	/* If we got a good PID, then
	 we can exit the parent process. */

	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	config = buildConfig();
	config->port = 1080;
	config->logfile = "ihttpd.log";
	config->rootdir = getCurrentDir();
	config->moddir = calloc(1, strlen(config->rootdir) + 5);
	strcpy(config->moddir, config->rootdir);
	strcat(config->moddir, "/mod");

	int c;

	while (1) {
		static struct option long_options[] = {
		/* These options set a flag. */
		{ "rootdir", required_argument, 0, 'r' }, { "moddir", required_argument,
				0, 'm' }, { "port", required_argument, 0, 'p' }, { "logfile",
		required_argument, 0, 'l' }, { "config", required_argument, 0, 'c' }, {
				"help", no_argument, 0, 'h' }, { 0, 0, 0, 0 } };
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long(argc, argv, "r:m:p:l:c:h", long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		int pathExists;
		int portOpt;

		switch (c) {
		case 0:
			/* If this option set a flag, do nothing else now. */
			if (long_options[option_index].flag != 0)
				//printf("This is the 0.\n");
				break;
			break;

		case 'r':
			pathExists = verifyResource(optarg);
			if (pathExists == 2) {
				config->rootdir = optarg;
			} else {
				printf("Could not open root directory.  Exiting.\n");
				return 0;
			}
			break;

		case 'm':
			pathExists = verifyResource(optarg);
			if (pathExists == 2) {
				config->moddir = optarg;
			} else {
				printf("Could not open mod directory.  Exiting.\n");
				return 0;
			}
			break;

		case 'p':
			portOpt = parseInt(optarg);
			if (portOpt > 0) {
				config->port = portOpt;
			} else {
				printf("Please provide a valid port number.\n");
				return 0;
			}
			break;

		case 'l':
			config->logfile = optarg;
			break;

		case 'c':
			pathExists = verifyResource(optarg);
			if (pathExists == 1) {
				readConfigFile(optarg, config);
			} else {
				printf("Could not read config file.  Exiting.\n");
				return 0;
			}
			break;

		case 'h':
			printf("My http server, version 1.0\n");
			printf("Possible flags: \n");
			printf("-r, --rootdir    override the default root directory\n");
			printf("-m, --moddir     override the default mod directory\n");
			printf("-p, --port       override the default port\n");
			printf(
					"-c, --config	 override the configuration with a config file");
			printf("-h, --help       show help\n");
			return 0;
			break;

		case '?':
			/* getopt_long already printed an error message. */
			return 0;
			break;

		default:
			abort();
		}
	}

	if (optind < argc) {
		printf("Command not recognized.  Exiting.\n");
		return 0;
	}

	int logFileLock = open(config->logfile, O_RDWR | O_CREAT | O_APPEND, 0744);
	int lockmode = LOCK_EX;

	if (logFileLock < 0) {
		fprintf(stderr, "Could not open file %s\n", config->logfile);
		exit(EXIT_FAILURE);
	}

	fprintf(stdout, "trying to lock %s (%s)\n", config->logfile,
			lockmode == LOCK_EX ? "exclusive" : "shared");
	flock(logFileLock, lockmode);
	fprintf(stdout, "successfully locked %s\n", config->logfile);

	int socket_desc, client_sock, s, *new_sock;
	struct sockaddr_in server, client;

	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1) {
		printf("Could not create socket\n");
	}
	puts("Socket created\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(config->port);

	//Bind
	if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
		//print the error message
		perror("bind failed. Error\n");
		return 1;
	}
	puts("bind done");

	//Listen
	listen(socket_desc, 3);

	//Accept and incoming connection
	puts("Waiting for incoming connections...\n");
	s = sizeof(struct sockaddr_in);

	while ((client_sock = accept(socket_desc, (struct sockaddr *) &client,
			(socklen_t*) &s))) {
		puts("Connection accepted\n");

		pthread_t sniffer_thread;
		new_sock = malloc(4);
		*new_sock = client_sock;

		int tada = pthread_create(&sniffer_thread, NULL, processRequest,
				(void*) new_sock);
		if (tada < 0) {
			perror("could not create thread\n");
			return 1;
		}

		//Now join the thread , so that we dont terminate before the thread
		//pthread_join( sniffer_thread , NULL);
		puts("Handler assigned\n");
		//break;
	}

	if (client_sock < 0) {
		perror("accept failed\n");
		return 1;
	}

	return 0;
	//exit(0);
}

/*
 * http.h
 *
 *  Created on: Apr 19, 2014
 *      Author: armin
 */
#include <stdlib.h>
#include <stdio.h>

#include "stringutil.h"

#ifndef HTTP_H_
#define HTTP_H_

/*
 * The two accepted HTTP methods
 */
typedef enum {GET, HEAD} HTTPMethod;

/*
 * Object representing the incoming HTTP Request
 */
typedef struct HTTPRequest {
	HTTPMethod method;
	char *protocol;
	char *resource;
	char *host;
	char *headers;
	char *date;
} HTTPRequest;

/*
 * Object representing the outgoing HTTP Response
 */
typedef struct HTTPResponse {
	char *status;
	char *statusString;
	char *protocol;
	char *lastModified;
	unsigned long contentLength;
	char *contentType;
	char *content;
} HTTPResponse;

HTTPRequest *parseRequest(char *input);

HTTPResponse *buildResponse();

int responseSize(HTTPResponse *response);

void responseString(char *buffer, HTTPResponse *response, int isGet);

void destroyRequest(HTTPRequest *request);
void destroyResponse(HTTPResponse *response);

void generate404(HTTPResponse *response);

#endif /* HTTP_H_ */

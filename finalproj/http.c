/*
 * http.c
 *
 *  Created on: Apr 19, 2014
 *      Author: armin
 */
#include <string.h>

#include "http.h"
#include "stringutil.h"

/*
 * Build a Request object
 */
HTTPRequest *buildRequest() {
	HTTPRequest *request = calloc(1, sizeof(HTTPRequest));
	return request;
}

/*
 * Function that parses the incoming request
 */
HTTPRequest *parseRequest(char *input) {

	printf("The input is \n\n%s\n\n", input);
	int buffSize = strlen(input) + 200;
	printf("Size of input is %i\n", (int) strlen(input));
	char *buffer = calloc(1, buffSize);
	printf("Size of buffer is %i\n", (int) strlen(buffer));
	char *line = calloc(1, buffSize);
	printf("Size of line is %i\n", (int) strlen(line));

	strncpy(buffer, input, strlen(input));

	printf("The input is now \n\n%s\n\n", input);
	ssize_t read;

	HTTPRequest *request = buildRequest();
	read = getlineFromString(line, buffer);
	if (read != -1) {

		if (startsWith(line, "GET") == 0) {
			request->method = GET;
			// Chop GET plus the next space
			chopFrontCount(line, 4);
		} else if (startsWith(line, "HEAD") == 0) {
			request->method = HEAD;
			// Chop HEAD plus the next space
			chopFrontCount(line, 5);
		} else {
			free(buffer);
			free(line);
			printf("Completely invalid request 1!\n");
			return request;
		}

		char *resourceBuffer = calloc(1, strlen(line) + 1);
		if (readUntilChar(resourceBuffer, line, ' ') == 0) {
			request->resource = resourceBuffer;
		} else {
			free(buffer);
			free(line);
			printf("Completely invalid request 2!\n");
			return request;
		}

		char *protocolBuffer = calloc(1, strlen(line) + 1);
		strcpy(protocolBuffer, line);
		request->protocol = protocolBuffer;

	} else {
		printf("Completely invalid request 3!\n");
		free(buffer);
		free(line);
		return request;
	}

	while ((read = getlineFromString(line, buffer)) != -1) {
		char *lineBuffer = calloc(1, strlen(line) + 1);
		strcpy(lineBuffer, line);
		if (startsWith(lineBuffer, "User-Agent:") == 0) {
			printf("Starts with useragent\n");
		} else if (startsWith(lineBuffer, "Host:") == 0) {
			printf("Starts with Host\n");
			char *hostBuffer = calloc(1, strlen(lineBuffer) + 1);
			strcpy(hostBuffer, lineBuffer);
			request->host = hostBuffer;
		} else if (startsWith(lineBuffer, "Date:") == 0) {
			printf("Starts with Date\n");
			char *hostBuffer = calloc(1, strlen(lineBuffer) + 1);
			strcpy(hostBuffer, lineBuffer);
			request->host = hostBuffer;
		} else if (startsWith(lineBuffer, "Last-Modified:") == 0) {
			printf("Starts with Last-Modified\n");
			char *hostBuffer = calloc(1, strlen(lineBuffer) + 1);
			strcpy(hostBuffer, lineBuffer);
			request->host = hostBuffer;
		} else if (startsWith(lineBuffer, "Content-Length:") == 0) {
			printf("Starts with Content-Length\n");
			char *hostBuffer = calloc(1, strlen(lineBuffer) + 1);
			strcpy(hostBuffer, lineBuffer);
			request->host = hostBuffer;
		} else if (startsWith(lineBuffer, "Content-Type:") == 0) {
			printf("Starts with Content-Type\n");
			char *hostBuffer = calloc(1, strlen(lineBuffer) + 1);
			strcpy(hostBuffer, lineBuffer);
			request->host = hostBuffer;
		} else if (startsWith(lineBuffer, "Accept:") == 0) {
			printf("Starts with Accept:\n");
		}
		free(lineBuffer);
	}

	free(buffer);
	free(line);

	return request;

}

/*
 * Constructor for the Response object
 */
HTTPResponse *buildResponse() {

	HTTPResponse *response = calloc(1, sizeof(HTTPResponse));
	return response;
}

/*
 * Calculate the necessary size of the response
 */
int responseSize(HTTPResponse *response) {

	int size = strlen(response->status);
	size += strlen(response->statusString);
	size += strlen(response->protocol);
	size += strlen(response->lastModified);
	size += response->contentLength;
	size += strlen(response->contentType);
	//Give it extra room to account for spaces and \r\n in the string
	size += 1024;
	return size;
}

/*
 * Build the response output string
 */
void responseString(char *buffer, HTTPResponse *response, int isGet) {

	char *timeStamp = getDateStamp();
	char contentSizeString[33];
	sprintf(contentSizeString, "%lu", response->contentLength);
	strcpy(buffer, response->protocol);
	strcat(buffer, " ");
	strcat(buffer, response->status);
	strcat(buffer, " ");
	strcat(buffer, response->statusString);
	strcat(buffer, "\r\n");
	strcat(buffer, "Date: ");
	strcat(buffer, timeStamp);
	strcat(buffer, "\r\n");
	strcat(buffer, "Last-Modified: ");
	strcat(buffer, response->lastModified);
	strcat(buffer, "\r\n");
	strcat(buffer, "Content-Length: ");
	strcat(buffer, contentSizeString);
	strcat(buffer, "\r\n");
	strcat(buffer, "Content-Type: ");
	strcat(buffer, response->contentType);
	strcat(buffer, "\r\n");
	if (isGet == 1) {
		strcat(buffer, "\r\n");
	}

}

/*
 * The request destructor
 */
void destroyRequest(HTTPRequest *request) {

	free(request->protocol);
	free(request->resource);
	free(request->host);
	free(request->headers);
	free(request->date);
	free(request);
}

/*
 * The response destructor
 */
void destroyResponse(HTTPResponse *response) {

	free(response->lastModified);
	free(response->content);
	free(response);
}

/*
 * Generate a 404 response
 */
void generate404(HTTPResponse *response) {

	response->status = "404";
	response->statusString = "Not Found";
	response->lastModified = calloc(1, 200);
	strcpy(response->lastModified, getDateStamp());
	response->contentType = "text/html";
	response->content = calloc(1, 200);
	strcpy(response->content,
			"<p><b>HTTP 400</b> The resource requested could not be found</p>\r\n");
	response->contentLength = strlen(response->content);
}


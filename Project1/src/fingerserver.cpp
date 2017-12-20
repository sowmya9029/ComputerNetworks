/*
 **Author: Sowmya
 **Version: 1.0
 **Created on: Sep 29, 2017
 **Name: fingerserver.cpp
 **References :http://beej.us/guide/bgnet/output/html/multipage/clientserver.html
 **Description : connects to multiple clients and returns the response to client request for finger input
 **
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>


#define PORT "10222"  // the port users will be connecting to ( (1000)+((23-1)*10)
#define BACKLOG 10     // how many pending connections queue will hold
using namespace std;
const bool DEBUG = false;
const string FINGER_LOCATION = "/usr/bin/finger";
const string FINGER_COMMAND = "finger";

int main(void)
{
	string data;

	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // client addr.
	sockaddr *sockaddr;
	sockaddr_in * inetAddr;
	sockaddr_in6 * inet6Addr;
	socklen_t sin_size;
	char buffer[1024] = {0};
	string response;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;


	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo);

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	if(DEBUG)
		printf("server: waiting for connections...\n");

	while(1) { //polling for connections
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}
		sockaddr = (struct sockaddr *)&their_addr;
		if(sockaddr->sa_family == AF_INET) {
			inetAddr = (sockaddr_in *)sockaddr;
		inet_ntop(their_addr.ss_family,
				inetAddr,
				s, sizeof s);
		}
		else {
				inet6Addr = (sockaddr_in6 *)sockaddr;
				inet_ntop(their_addr.ss_family,
						inet6Addr, s, sizeof s);
		}

		if(DEBUG)
			printf("server: got connection from %s\n", s);

		if (!fork()) {
			close(sockfd); //close parent socket.
			read( new_fd , buffer, 1024);
			if(DEBUG)
				printf(" got data %s\n",buffer );
			dup2(new_fd, 0);//standard input
			dup2(new_fd, 1); //standard output
			dup2(new_fd, 2); // standard error
			execl(FINGER_LOCATION.c_str(),FINGER_COMMAND.c_str(), buffer, NULL);
			exit(0);

			if(DEBUG)
				printf("%s\n",response.c_str() );
		}
		close(new_fd); //close child socket.
	}

	return 0;
}

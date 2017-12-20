/*
 **Author: Sowmya
 **Version: 1.0
 **Created on: Sep 29, 2017
 **Name: fingerclient.cpp
 **Description : sends connection request to the server for executing finger command
 **References :http://beej.us/guide/bgnet/output/html/multipage/clientserver.html
 **Assumptions: client enter  the commands  fingerclient username@hostname:server_port without and spaces and in the same format
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstdint>
using namespace std;
const bool DEBUG =false;

void readSocket(int sockfd)
{
	char buffer[1024] = {0};
	// read message from server
		int buflen,n;
		n = read(sockfd, (char*)&buflen, sizeof(buflen));
		if (n < 0)
			printf("ERROR reading from socket");
		buflen = ntohl(buflen);
		n = read(sockfd, buffer, buflen);
		if (n < 0)
			printf("ERROR reading from socket");
		else
			printf("%*.*s\n", n, n, buffer);
}
int main(int argc, char **argv) {
	struct addrinfo hints, *servinfo, *p;
	string commandarg="finger";
	int sockfd = 0,retval,sentBytes,valread;


	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if (argc < 2) {
		printf("Please enter proper arguments  eg:fingerclient username@hostname:20222\n");
		return -1;
	}
	char* command = argv[1];

	/**code for parsing command line data**/
	//  fingerclient username@hostname:server_port
	char *temp,*username,*hostname,*server_port;

	if(command)
	{
		temp = strtok(command, " ");
	}

	if(temp)
	{
		if(DEBUG)
			printf("%s\n", temp);
		username = strtok(temp, "@");
	}

	if(username)
	{
		if(DEBUG)
			printf("username::%s\n", username);
	}
	else
	{
		printf("Please enter proper username\n");
		return -1;
	}
	hostname = strtok(NULL, ":");
	if(hostname)
	{
		if(DEBUG)
			printf("hostname::%s\n", hostname);

	}
	else
	{
		printf("Please enter proper hostname\n");
		return -1;
	}
	server_port = strtok(NULL, "");
	if(server_port)
	{
		if(DEBUG)
			printf("server_port::%s\n", server_port);
	}
	else
	{
		printf("Please enter proper server port\n");
		return -1;
	}


	if((retval = getaddrinfo(hostname,server_port,&hints,&servinfo)) != 0) {
		printf("\n get addr connection error \n");
		return -1;
	}


	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}


	uint16_t len = strlen(username);
	uint16_t networkLen = htons(len); // convert to network byte order
	if((sentBytes = send(sockfd,username, networkLen,0)) != -1) {
		if(DEBUG)
			printf("sent data");
	}
	readSocket(sockfd);
	close(sockfd);
	// improve
	// char msg[1024];
	/*if((valread=read(sockfd,buffer, sizeof(buffer)))==-1) {     // read message from server
			cout<<"Read error"<<endl;
			exit(1);
		}
		printf("%s\n",buffer );*/

}


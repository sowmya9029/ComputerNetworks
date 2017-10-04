/*
 * fingerclient.cpp
 *
 *  Created on: Sep 29, 2017
 *      Author: sowmya satyavada
 */
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
using namespace std;
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main(int argc, char **argv) {
	struct addrinfo hints, *servinfo, *p;
	string commandarg="finger";
	int sockfd = 0,retval,sentBytes,valread;
	char buffer[1024] = {0};

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
//	if (argc < 2) {
//		return -1;
//	}
//	char* command = argv[1];
//
//	/**code for parsing command line data**/
//	//  fingerclient username@hostname:server_port
//	char *temp ,*username,*hostname,*server_port;
//	temp = strtok(command, " ");
//
//	if(temp)
//	{
//		printf("%s\n", temp);
//	}
//	username = strtok(NULL, "@");
//	if(username)
//	{
//		printf("username::%s\n", username);
//	}
//	hostname = strtok(NULL, ":");
//	if(hostname)
//	{
//		printf("hostname::%s\n", hostname);
//	}
//	server_port = strtok(NULL, "");
//	if(server_port)
//	{
//		printf("server_port::%s\n", server_port);
//	}
//    if(hostname&&server_port)
//    {
	if((retval = getaddrinfo("","22880",&hints,&servinfo)) != 0) {
		printf("\n get addr connection error \n");
		return -1;
	}
//    }

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
//	if(username)
//	{
	if((sentBytes = send(sockfd,"finger sree", 13,0)) != -1) {
//			printf("sent data");
	}
	  if((valread=read(sockfd,buffer, 1024))==-1) {     // read message from server
	       cout<<"Read error"<<endl;
	       exit(1);
	    }
	printf("%s\n",buffer );
//	}
}


/************************
*	Author: Matt Mayberry
*	Date: October 30, 2016
*	CS372_400_F2016
*	Project 1
*   chat_client.c
*************************/

// References: http://beej.us/guide/bgnet/output/html/multipage/clientserver.html

#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>


#define MAXMESSAGESIZE 500 // send messages of up to 500 characters 
#define MAXHANDLESIZE 10 // max number of bytes allowed in client's handle


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int setupConnect(char* hostname, char* portnum){
	int sockfd;
    char buf[MAXMESSAGESIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN]; 
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(hostname, portnum, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

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
        exit(1);
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); 
	
	return sockfd;
}


void chat(int socket_fd, char* handle){
	char buf[MAXMESSAGESIZE];
	int numbytes;
	int quit;
	while(1){
	
		// send
		memset(buf, 0, MAXMESSAGESIZE);
		printf("%s> ", handle);
		fgets(buf, MAXMESSAGESIZE-1, stdin);
		quit = strncmp(buf, "\\quit", 4);
		
		if (quit == 0){
			if(send(socket_fd, "The client has closed the connection\n", 28, 0) == -1){
				perror("send");
				exit(1);
			}
			close(socket_fd);
			exit(0);
		}
		else{
			if(send(socket_fd, buf, strlen(buf), 0) == -1){
				perror("send");
				exit(1);
			}
		}
		
		// receive
		if ((numbytes = recv(socket_fd, buf, MAXMESSAGESIZE-1, 0)) == -1) {
			perror("recv");
			exit(1);
		}
		if (strncmp(buf, "Connection closed by Server", 27) == 0){
			printf("%s\n", buf);
			close(socket_fd);
			exit(0);
		}
		printf("%s", buf);
		printf("\n");
	}
	return;
}


int main(int argc, char *argv[])
{
    int sockfd, numbytes;  
    char buf[MAXMESSAGESIZE];
    char handle[MAXHANDLESIZE];
	size_t ln;
    
    if (argc != 3) {
        fprintf(stderr,"usage: client hostname portnumber\n");
        exit(1);
    }

	// connect to Server
	sockfd = setupConnect(argv[1], argv[2]);

	// get handle and send to Server
	printf("Handle: ");
	fgets(handle, MAXHANDLESIZE, stdin);
	ln = strlen(handle) - 1;
	if (handle[ln] == '\n')
    handle[ln] = '\0';
    if(send(sockfd, handle, strlen(handle), 0) == -1){
		perror("send");
		exit(1);
	}
	
	// Begin chat
	printf("\nWait for prompt to begin typing message\n");
	printf("Type '\\quit' to quit at any time\n\n");
	chat(sockfd, handle);      

    close(sockfd);

    return 0;
}

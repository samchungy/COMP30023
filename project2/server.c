/* A simple server in the internet domain using TCP
The port number is passed as an argument 


 To compile: gcc server.c -o server 
*/

#define MAX_CLIENTS 100
#define MAX_JOBS 10
#define PING "PING"
#define PONG "PONG"
#define OKAY "OKAY"
#define ERRO "ERRO"
#define SOLN "SOLN"
#define WORK "WORK"
#define ABRT "ABRT"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>


int main(int argc, char **argv)
{
	int sockfd, newsockfd, portno, clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	if (argc < 2) 
	{
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}

	 /* Create TCP socket */
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0) 
	{
		perror("ERROR opening socket");
		exit(1);
	}

	
	bzero((char *) &serv_addr, sizeof(serv_addr));

	portno = atoi(argv[1]);
	
	/* Create address we're going to listen on (given port number)
	 - converted to network byte order & any IP address for 
	 this machine */
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	/* store in machine-neutral format */
	serv_addr.sin_port = htons(portno);

	 /* Bind address to the socket */
	
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
			sizeof(serv_addr)) < 0) 
	{
		perror("ERROR on binding");
		exit(1);
	}
	
	/* Listen on socket - means we're ready to accept connections - 
	 incoming connection requests will be queued */
	
	listen(sockfd,MAX_CLIENTS);
	
	clilen = sizeof(cli_addr);

    while(1){
        newsockfd = accept(	sockfd, (struct sockaddr *) &cli_addr,
                               &clilen);
    }

	/* Accept a connection - block until a connection is ready to
	 be accepted. Get back a new file descriptor to communicate on. */



	if (newsockfd < 0) 
	{
		perror("ERROR on accept");
		exit(1);
	}
	bzero(buffer, sizeof(buffer));
	bzero(buffer,256);

	/* Read characters from the connection,
		then process */
	
	n = read(newsockfd,buffer,255);
	char header[256];
	char header2[5];
	char payload[256];
	char reply[256];
	bzero(reply,256);
	bzero(header,5);
	bzero(payload,256);

	sscanf(buffer,"%s%s",header, payload);
	if(strcmp(header,PING)==0){
		strcpy(reply,PONG);
	}
	else if(strcmp(header,PONG)==0){
        strcpy(reply,ERRO);
        strcat(reply, " reason: PONG messages are strictly reserved for server responses.");
	}
    else if(strcmp(header, OKAY)==0){
        strcpy(reply,ERRO);
        strcat(reply, " reason: It is not okay to send OKAY messages to the server, m'kay?");
    }
    else if (strcmp(header, ERRO)==0) {
        strcpy(reply, ERRO);
        strcat(reply, " reason: ERRO messages should not be sent to the server");
    }
    else if (strcmp(header,SOLN)==0){
        /*todo*/
    }
    else if (strcmp(header,WORK)==0){
        /*todo*/
    }
    else if (strcmp(header,ABRT)==0){
        /*todo*/
        strcpy(reply, OKAY);
    }
    else{
        /*todo*/

    }

	if (n < 0) 
	{
		perror("ERROR reading from socket");
		exit(1);
	}
	
	printf("Here is the message: %s\n",buffer);

	n = write(newsockfd,reply,strlen(reply));
	
	if (n < 0) 
	{
		perror("ERROR writing to socket");
		exit(1);
	}
	
	/* close socket */
	
	close(sockfd);
	
	return 0; 
}

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
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <time.h>
#include <inttypes.h>

void logActivity(char *, char *, int);
void initLogFile();
char * timestamp();


int main(int argc, char **argv)
{
	int sockfd, newsockfd, portno, clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int n, sockid = 0;
    initLogFile();

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
	
	if(listen(sockfd,MAX_CLIENTS)==0){
		printf("Listening\n");
	}
	else{
		printf("Error\n");
	}
	
	clilen = sizeof(cli_addr);

    while(1) {
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        sockid++;

		if (!fork()){
			n = 1;
			/*Loop while connection is still alive*/
			while(n!=0){
				if (newsockfd < 0)
				{
					perror("ERROR on accept");
					exit(1);
				}
				char buffer[256];
				bzero(buffer, sizeof(buffer));
				bzero(buffer,256);

				/* Read characters from the connection,
                    then process */

				n = read(newsockfd,buffer,255);
				char header[5];
				char reply[256];
				bzero(reply,256);
                uint32_t difficulty;
                char seed[65];
                uint64_t start;
                uint8_t worker_count;


				sscanf(buffer,"%s%"SCNu32"%s%"SCNu64"%"SCNu8"",header,
                       &difficulty,seed,&start,&worker_count);
				if(strcmp(header,PING)==0){
					strcpy(reply,PONG);
				}
				else if(strcmp(header,PONG)==0){
					strcpy(reply,ERRO);
					strcat(reply, " reason: PONG messages are strictly "
							"reserved for server responses.");
				}
				else if(strcmp(header, OKAY)==0){
					strcpy(reply,ERRO);
					strcat(reply, " reason: It is not okay to send OKAY "
							"messages to the server, m'kay?");
				}
				else if (strcmp(header, ERRO)==0) {
					strcpy(reply, ERRO);
					strcat(reply, " reason: ERRO messages should not be sent "
							"to the server");
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
                char clntName[INET_ADDRSTRLEN];
                if(inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,clntName,sizeof(clntName))!=NULL){
                    logActivity(reply, clntName, ntohs(cli_addr.sin_port));
                } else {
                    perror("ERROR reading IP of client\n");
                    exit(1);
                }

				n = write(newsockfd,reply,strlen(reply));

				if (n < 0)
				{
					perror("ERROR writing to socket");
					exit(1);
				}

				/* close socket */

				close(newsockfd);
				exit(0);
			}
		}
		else{
			close(newsockfd);
		}
	}

	/* Accept a connection - block until a connection is ready to
	 be accepted. Get back a new file descriptor to communicate on. */

	close(sockfd);
	return 0; 
}

void logActivity(char *reply, char *ip, int sockid){
    time_t ltime; /* calendar time */
    ltime=time(NULL); /* get current cal time */
    FILE *pfile;
    pfile = fopen("log.txt", "a");
    if (pfile!=NULL){
        fprintf(pfile,"%s - IP: %s SID: %d %s\n",asctime(localtime(&ltime)), ip, sockid, reply);
        fclose(pfile);
    }
    else{
        perror("ERROR appending log file");
        exit(1);
    }
}

void initLogFile(){
    FILE *pfile;
    pfile = fopen("log.txt", "w");
    if(pfile != NULL){
        fclose(pfile);
    }
}

char * timestamp()
{

    return ;
}
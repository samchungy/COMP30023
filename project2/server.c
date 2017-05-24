/* A simple server in the internet domain using TCP
The port number is passed as an argument 


 To compile: gcc server.c -o server 
*/

#define MAX_CLIENTS 100
#define MAX_JOBS 10
#define HEADER_SIZE 4
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
#include <pthread.h>

struct con_handle{
    struct sockaddr_in *sockadd;
    int sockfd;
};

void logActivity(char *, char *, int);
void initLogFile();
char * timestamp();
void *connection_handler(void *);
struct con_handle *createcon_handle(struct sockaddr_in *, int);

int main(int argc, char **argv)
{
	int sockfd, newsockfd, portno, clilen;
	struct sockaddr_in serv_addr, cli_addr;
    struct con_handle *ch;
	int sockid = 0;
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
    pthread_t thread_id;

    while(newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) {
        printf("Accept Connection\n");
        ch = createcon_handle(&cli_addr, newsockfd);
        if (pthread_create(&thread_id, NULL, connection_handler, ch) != 0) {
            perror("could not create thread");
            return 1;
        }
        /*pthread_join( thread_id , NULL);*/
        pthread_detach(thread_id);
        puts("Handler assigned");
    }
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

struct con_handle *createcon_handle(struct sockaddr_in *sockadd, int sockfd){
    struct con_handle *ch = malloc(sizeof(ch));
    ch->sockadd = sockadd;
    ch->sockfd = sockfd;
    return ch;
}


void *connection_handler(void *connect_handle) {
    /*Get the socket descriptor*/
    int n;
    struct con_handle *ch = (struct con_handle *)connect_handle;
    int sock = ch->sockfd;
    struct sockaddr_in cli_addr = *(ch->sockadd);
    int read_size;
    char header[5];
    char reply[256];
    char buffer[256];
    uint32_t difficulty;
    char seed[65];
    uint64_t start;
    uint8_t worker_count;

        /*Receive a message from client*/
        while(n = recv(sock,buffer,128,0) > 0) {
            bzero(reply,256);
            /*sscanf(buffer,"%s%"SCNu32"%s%"SCNu64"%"SCNu8"",header,
                   &difficulty,seed,&start,&worker_count);*/
            strncpy(header,buffer,HEADER_SIZE);
            if(strcmp(header,"PING")==0){
                strcpy(reply,PONG);
            }
            else if(strcmp(header,"PONG")==0){
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
                strcpy(reply, ERRO);
                strcat(reply, " reason: ");
                strcat(reply, header);
                strcat(reply, " is an undefined protocol message."
                        " Please enter a proper message.");

            }

            char clntName[INET_ADDRSTRLEN];
            if(inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,clntName,
                         sizeof(clntName))!=NULL){
                logActivity(reply, clntName, ntohs(cli_addr.sin_port));
            } else {
                perror("ERROR reading IP of client\n");
                exit(1);
            }
            strcat(reply,"\r\n");
            n = write(sock,reply,strlen(reply));
            bzero(buffer,256);

        }

        if(n == 0)
        {
            puts("Client disconnected");
            fflush(stdout);
        }
        if(n < 0) {
            perror("recv failed");
        }
        free(connect_handle);
        close(sock);
        return 0;
}
/* Sam Chung (758 053) Project 2 Computer Systems
*/

#include "server.h"

int main(int argc, char **argv)
{
	int sockfd, newsockfd, portno, clilen;
	struct sockaddr_in serv_addr, cli_addr;
    struct con_handle *ch;
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

    while((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen))){
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

void scan_section(char msg[], char buffer[]){
    bzero(msg,BUFFER_SIZE);
    int i, b_size = strlen(buffer);
    char c = '\0', last_c;
    for (i=0;i<b_size;i++){
        last_c = c;
        c = buffer[i];
        if (c == ' ' || (last_c == '\r' && c == '\n')){
            if (c == ' '){
                msg[i] = '\0';
            }
            else{
                msg[i-1] = '\0';
            }
            /*End Case*/
            break;
        }
        msg[i] = c;
    }
    if (i == b_size){
        msg[i] = '\0';
    }
}


void *connection_handler(void *connect_handle) {
    /*Get the socket descriptor*/
    int n, b_size;
    struct con_handle *ch = (struct con_handle *)connect_handle;
    int sock = ch->sockfd;
    struct sockaddr_in cli_addr = *(ch->sockadd);
    char reply[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    char msg[BUFFER_SIZE];
    char diff[DIFFICULTY_SIZE+1];
    char seed[65];
    uint64_t start;
    uint8_t worker_count;

        /*Receive a message from client*/
        while((n = recv(sock,buffer,BUFFER_SIZE-1,0) > 0)) {
            bzero(reply,BUFFER_SIZE);
            bzero(msg,BUFFER_SIZE);
            b_size = strlen(buffer);

            /*Scan for Header Message*/
            scan_section(msg, buffer);

            if (strlen(msg) != HEADER_SIZE){
                strcpy(reply,ERRO);
                strcat(reply, " reason: Undefined protocol message.");
            }
            else if(strcmp(msg,PING)==0){
                strcpy(reply,PONG);
            }
            else if(strcmp(msg,PONG)==0){
                strcpy(reply,ERRO);
                strcat(reply, " reason: PONG messages are strictly "
                        "reserved for server responses.");
            }
            else if(strcmp(msg, OKAY)==0){
                strcpy(reply,ERRO);
                strcat(reply, " reason: It is not okay to send OKAY "
                        "messages to the server, m'kay?");
            }
            else if (strcmp(msg, ERRO)==0) {
                strcpy(reply, ERRO);
                strcat(reply, " reason: ERRO messages should not be sent "
                        "to the server");
            }
            else if (strcmp(msg,SOLN)==0){
                if (b_size <= 5){
                    strcpy(reply, ERRO);
                    strcat(reply, " reason: SOLN requires more information");
                }
                else{
                    /*Scan for Difficulty*/
                    scan_section(msg,buffer);
                    strcpy(reply, d);
                    strcat(reply, " reason: SOLN requires more information");
                }

            }
            else if (strcmp(msg,WORK)==0){
                /*todo*/
            }
            else if (strcmp(msg,ABRT)==0){
                /*todo*/
                strcpy(reply, OKAY);
            }
            else{
                strcpy(reply, ERRO);
                strcat(reply, " reason: Undefined protocol message.");

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
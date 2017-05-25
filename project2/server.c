/* Sam Chung (758 053) Project 2 Computer Systems
*/

#include "server.h"
#include "Work.h"
#include "soln.h"

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
	}
	else{
		printf("Error\n");
	}
	
	clilen = sizeof(cli_addr);
    pthread_t thread_id;

    while((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen))){
        ch = createcon_handle(&cli_addr, newsockfd);
        if (pthread_create(&thread_id, NULL, connection_handler, ch) != 0) {
            perror("could not create thread");
            return 1;
        }
        /*pthread_join( thread_id , NULL);*/
        pthread_detach(thread_id);
    }
    return 0;
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

struct worker *new_worker(char *diff, char *seed, char *sol, int sock){
    struct worker *w = malloc(sizeof(struct worker));
    char *d = (char *)malloc((strlen(diff)+1)*sizeof(char));
    char *s = (char *)malloc((strlen(seed)+1)*sizeof(char));
    char *so = (char *)malloc((strlen(sol)+1)*sizeof(char));
    strcpy(d,diff);
    strcpy(s,seed);
    strcpy(so,sol);
    w->diff = d;
    w->seed = s;
    w->sol = so;
    w->sock = sock;
    return w;
}

void scan_section(char msg[], char buffer[], int start){
    bzero(msg,BUFFER_SIZE);
    int i, j=0, b_size = strlen(buffer);
    char c = '\0', last_c;
    for (i=start;i<b_size;i++){
        last_c = c;
        c = buffer[i];
        if (c == ' ' || (last_c == '\r' && c == '\n')){
            if (c == ' '){
                msg[j] = '\0';
            }
            else{
                msg[j-1] = '\0';
            }
            /*End Case*/
            break;
        }
        msg[j++] = c;
    }
    if (i == b_size && c != '\n'){
        msg[i] = '\0';
    }
}

void erro_msg(char reply[], char *error){
    int i;
    strcat(reply,ERRO);
    strcat(reply,error);
    for(i=strlen(reply);i<ERROR_LEN+HEADER_SIZE-1;i++){
        reply[i] = ' ';
    }
    reply[i]='\0';
}

void logActivity(char *ip, int sockid, char *buff){
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    FILE *pfile;
    char time[strlen(asctime (timeinfo))];
    strcpy(time,asctime (timeinfo));
    time[strlen(asctime (timeinfo))-1]='\0';
    pfile = fopen("log.txt", "a");
    if (pfile!=NULL){
        fprintf(pfile,"[%s]        %s(%d): %s\n",
                time,ip,sockid, buff);
        fclose(pfile);
    }
    else{
        perror("ERROR appending log file");
        exit(1);
    }
}

void logwork(char *buff){
    FILE *pfile;
    pfile = fopen("log.txt", "a");
    if (pfile!=NULL){
        fprintf(pfile,"%s\n",buff);
        fclose(pfile);
    }
    else{
        perror("ERROR appending log file");
        exit(1);
    }
}

void *connection_handler(void *connect_handle) {
    /*Get the socket descriptor*/
    int n, b_size;
    struct con_handle *ch = (struct con_handle *)connect_handle;
    int sock = ch->sockfd;
    struct sockaddr_in cli_addr = *(ch->sockadd);
    char msg[BUFFER_SIZE];
    char header[HEADER_SIZE+1];
    char reply[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    char diff[DIFFICULTY_SIZE+1];
    char sol[SOL_SIZE+1];
    char seed[SEED_SIZE+1];
    char clntName[INET_ADDRSTRLEN];

    inet_ntop(AF_INET,&cli_addr.sin_addr.s_addr,clntName, sizeof(clntName));
    logActivity(clntName, sock, "Session Started.");

        /*Receive a message from client*/
        while((n = recv(sock,buffer,BUFFER_SIZE-1,0) > 0)) {
            bzero(reply,BUFFER_SIZE);
            bzero(msg,BUFFER_SIZE);
            struct worker *w;
            b_size = strlen(buffer);

            /*Scan for Header Message*/
            scan_section(msg, buffer, 0);
            logActivity(clntName, sock, buffer);

            if (strlen(msg) != HEADER_SIZE){
                erro_msg(reply," Undefined protocol message");
            }
            else if(strcmp(msg,PING)==0){
                strcpy(reply,PONG);
            }
            else if(strcmp(msg,PONG)==0){
                erro_msg(reply," PONG msgs are for server responses");
            }
            else if(strcmp(msg, OKAY)==0){
                erro_msg(reply," Not Okay to sent OKAY");
            }
            else if (strcmp(msg, ERRO)==0) {
                erro_msg(reply," ERRO msg cannot be sent");
            }
            else if (strcmp(msg,SOLN)==0 || strcmp(msg,WORK)==0){
                strcpy(header,msg);
                if (b_size <= 6){
                    erro_msg(reply, " invalid msg");
                }
                else{
                    /*Scan for Difficulty*/
                    scan_section(msg,buffer, HEADER_SIZE+1);
                    if ((strlen(msg)) != DIFFICULTY_HEX){
                        erro_msg(reply," invalid difficulty size");
                    }
                    else{
                        strcpy(diff, msg);
                        scan_section(msg,buffer,HEADER_SIZE+1+DIFFICULTY_HEX+1);
                        if ((strlen(msg) != SEED_SIZE)){
                            erro_msg(reply, " invalid seed size");
                        }
                        else{
                            strcpy(seed,msg);
                            scan_section(msg,buffer, HEADER_SIZE+1+
                               DIFFICULTY_HEX+1+SEED_SIZE+1);
                            strcpy(sol,msg);
                            if ((strlen(msg)!=SOL_SIZE)){
                                erro_msg(reply, " invalid sol size");
                            }
                            else{
                                if((strcmp(header,SOLN)==0)){
                                    calculate_soln(reply, diff, seed, sol);
                                }
                                else{
                                    scan_section(msg,buffer, HEADER_SIZE+1+
                                  DIFFICULTY_HEX+1+SEED_SIZE+1+SOL_SIZE+1);
                                    w = new_worker(diff,seed,sol, sock);
                                    pthread_t workerthread;
                                    if (pthread_create(&workerthread, NULL,
                                                       calculate_work, w) != 0){
                                        perror("could not create thread");
                                        return 1;
                                    }
                                    pthread_detach(workerthread);
                                }
                            }

                        }
                    }

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
                erro_msg(reply," Undefined protocol message.");

            }
            if(strlen(reply)!=0){
                logActivity("0.0.0.0", sock, reply);
                strcat(reply,"\r\n");
                n = write(sock,reply,strlen(reply));
            }
            bzero(buffer,256);

        }

        if(n < 0) {
            perror("recv failed");
        }
        free(connect_handle);
        close(sock);
        return 0;
}
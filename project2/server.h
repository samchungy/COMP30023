//
// Created by SamCh on 24/05/2017.
//

#ifndef PROJECT2_SERVER_H
#define PROJECT2_SERVER_H

#define MAX_CLIENTS 100
#define MAX_JOBS 10
#define HEADER_SIZE 4
#define BUFFER_SIZE 256
#define DIFFICULTY_SIZE 32

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
#include <arpa/inet.h>

struct con_handle{
    struct sockaddr_in *sockadd;
    int sockfd;
};

void logActivity(char *, char *, int);
void initLogFile();
char * timestamp();
void scan_section(char *, char *);
void *connection_handler(void *);
struct con_handle *createcon_handle(struct sockaddr_in *, int);

#endif //PROJECT2_SERVER_H

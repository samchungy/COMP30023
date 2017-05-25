//
// Created by SamCh on 24/05/2017.
//

#ifndef PROJECT2_SERVER_H
#define PROJECT2_SERVER_H

#define MAX_CLIENTS 100
#define MAX_JOBS 10
#define HEADER_SIZE 4
#define BUFFER_SIZE 256
#define DIFFICULTY_HEX 8
#define DIFFICULTY_SIZE 32
#define ERROR_LEN 40
#define SEED_SIZE 64
#define SOL_SIZE 16
#define BYTE_SIZE 32
#define BIG_BYTE 40

#define PING "PING"
#define PONG "PONG"
#define OKAY "OKAY"
#define ERRO "ERRO"
#define SOLN "SOLN"
#define WORK "WORK"
#define ABRT "ABRT"
#define HEX "0x"

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

struct worker{
    char *diff;
    char *seed;
    char *sol;
};
void logActivity(char *, int, char *);
void initLogFile();
char * timestamp();
void scan_section(char [], char [], int);
void *connection_handler(void *);
void erro_msg(char [], char *);
struct con_handle *createcon_handle(struct sockaddr_in *, int);
void logwork(char *);
struct worker *new_worker(char *, char *, char *);

#endif //PROJECT2_SERVER_H

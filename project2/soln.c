//
// Created by SamCh on 25/05/2017.
//

#include "soln.h"
#include "server.h"
#include "sha256.h"

void calculate_soln(char *reply, char* diff, char* seed, char *sol){
    char alpha_s[5] = HEX, beta_s[7], concat_s[strlen(seed)+strlen(sol)+1];
    char log[BUFFER_SIZE];
    char *logbytes;
    bzero(beta_s, 7);
    BYTE array_u[32],result_u[32],clean_u[32],beta_u[32],concat_u[40];
    uint32_t alpha, ex;
    uint256_init(array_u);
    uint256_init(result_u);
    uint256_init(beta_u);
    uint256_init(clean_u);
    strcpy(concat_s,seed);
    strcat(concat_s,sol);

    hex_to_byte(40, concat_u, concat_s);
    strncat(alpha_s, diff, 2);
    strcat(beta_s, diff+2);
    alpha = strtoul(alpha_s, NULL, 16);
    ex = 8*(alpha-3);
    hex_to_byte(32, beta_u, beta_s);
    array_u[31] = 0x02;
    uint256_exp(clean_u,array_u,ex);
    uint256_mul(result_u,clean_u,beta_u);
    print_uint256(result_u);
    logbytes = printbytes(result_u,32);
    strcpy(log,"TARGET: ");
    strcat(log,logbytes);
    logwork(log);
    free(logbytes);

    if(doublehash(concat_u, result_u, log)==-1){
        strcat(reply, OKAY);
    }
    else{
        erro_msg(reply, " not a valid proof of work");
    }

}

void hex_to_byte(int n, BYTE *num_u, char *string){
    int i, j=n-(strlen(string)/2);
    int lenstr = strlen(string);
    char temp_s[5];
    uint32_t temp;
    for (i=0; i<lenstr; i=i+2){
        bzero(temp_s, 5);
        strcat(temp_s,HEX);
        strncat(temp_s,string+i,2);
        temp = strtol(temp_s, NULL, 16);
        num_u[j] = temp;
        j++;
    }
}

int doublehash(BYTE *bytes, BYTE *target, char *log){
    char *logbytes;
    BYTE buf[SHA256_BLOCK_SIZE];
    BYTE buf2[SHA256_BLOCK_SIZE];
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, bytes, 40);
    sha256_final(&ctx, buf);
    sha256_init(&ctx);
    sha256_update(&ctx, buf, 32);
    sha256_final(&ctx, buf2);
    logbytes = printbytes(buf2,32);


    if (sha256_compare(buf2,target) == -1) {
        strcpy(log, "HASH: ");
        strcat(log, logbytes);
        free(logbytes);
        logwork(log);
        return -1;
    }
    else{
        return 1;
    };

}

char *printbytes (BYTE *bytes, int num) {
    int i;
    char *byte_s = (char *)malloc(num*2+3);
    char temp[5];
    strcpy(byte_s, HEX);
    for (i = 0; i < num; i++) {
        sprintf (temp,"%02x", bytes[i]);
        strcat(byte_s,temp);
    }
    return byte_s;
}
//
// Created by SamCh on 24/05/2017.
//

#include "Work.h"
#include "soln.h"
#include "server.h"


void * calculate_work(void *work){
    struct worker w = *(work);
    char alpha_s[5] = HEX, beta_s[7], concat_s[strlen(w->seed)+strlen(w->sol)+1];
    char log[BUFFER_SIZE];
    char *logbytes;
    int usetemp = 0;
    bzero(beta_s, 7);
    BYTE array_u[32],result_u[32],clean_u[32],beta_u[32],concat_u[40],
            single_u[40], concat2_u[40];
    uint32_t alpha, ex;
    uint256_init(array_u);
    uint256_init(result_u);
    uint256_init(beta_u);
    uint256_init(clean_u);
    uint256_init_40(concat_u);
    uint256_init_40(single_u);
    uint256_init_40(concat2_u);
    strcpy(concat_s,w->seed);
    strcat(concat_s,w->sol);

    single_u[39]=0x01;
    hex_to_byte(40, concat_u, concat_s);
    strncat(alpha_s, w->diff, 2);
    strcat(beta_s, w->diff+2);
    alpha = strtoul(alpha_s, NULL, 16);
    ex = 8*(alpha-3);
    hex_to_byte(32, beta_u, beta_s);
    array_u[31] = 0x02;
    uint256_exp(clean_u,array_u,ex);
    uint256_mul(result_u,clean_u,beta_u);
    logbytes = printbytes(result_u,32);
    strcpy(log,"TARGET: ");
    strcat(log,logbytes);
    logwork(log);
    free(logbytes);

    while(1){
        if (usetemp){
            if(doublehash(concat2_u, result_u, log)==-1){
                logbytes = get_solution(concat2_u);
                break;
            }else{
                uint256_add_40(concat_u,concat2_u,single_u);
            }
            usetemp = 0;
        }
        else{
            if(doublehash(concat_u, result_u, log)==-1){
                logbytes = get_solution(concat_u);
                break;
            }else{
                uint256_add_40(concat2_u,concat_u,single_u);
            }
            usetemp = 1;
        }
    }
    sprintf(reply,"%s %s %s %s",SOLN, w->diff, w->seed, logbytes);
    free(logbytes);
}


char * get_solution(BYTE *concat){
    int i;
    char *byte_s = (char *)malloc(SOL_SIZE+1);
    char temp[5];
    bzero(byte_s,SOL_SIZE+1);
    for (i = BYTE_SIZE; i < BIG_BYTE; i++) {
        sprintf (temp,"%02x", concat[i]);
        strcat(byte_s, temp);
    }
    return byte_s;
}

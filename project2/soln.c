//
// Created by SamCh on 25/05/2017.
//

#include "soln.h"
#include "server.h"

void calculate_soln(char *reply, char* diff, char* seed, char *sol){
    char alpha_s[5] = HEX, beta_s[7];
    bzero(beta_s, 7);
    BYTE array[32], result[32], clean[32], beta[32];
    uint32_t alpha, ex;


    printf("%s,%s,%s\n", diff, alpha_s, beta_s);
    strncat(alpha_s, diff, 2);
    strcat(beta_s, diff+2);
    printf("%s, %s\n", alpha_s, beta_s);

    alpha = strtoul(alpha_s, NULL, 16);
    ex = 8*(alpha-3);

    uint256_init(array);
    array[31] = 0x02;
    uint256_init(result);
    uint256_init(beta);
    uint256_exp(result,array,ex);

    printf("%" PRIu32 "\n",alpha);
    printf("%" PRIu32 "\n",ex);

    print_uint256(array);
    print_uint256(result);
    print_uint256(clean);
    hex_to_uint256(beta, beta_s);
    print_uint256(beta);

}

void hex_to_uint256(BYTE *num, char *string){
    int i, j=29;
    int lenstr = strlen(string);
    char temp_s[5];
    uint32_t temp;
    for (i=0; i<lenstr; i=i+2){
        bzero(temp_s, 5);
        strcat(temp_s,HEX);
        strncat(temp_s,string+i,2);
        temp = strtol(temp_s, NULL, 16);
        num[j] = temp;
        j++;
    }
}
#include "swap.h"
#include "process.h"

pronode_t * read_from_file(char *filename, int *numprocesses, pronode_t *head){
    int time_cr;
    int pr_id;
    int mem_size;
    int job_time;
    FILE *file = fopen (filename, "r");
    while(fscanf(file,"%d",&time_cr)!=EOF){
      fscanf(file,"%d",&pr_id);
      fscanf(file,"%d",&mem_size);
      fscanf(file,"%d",&job_time);
      *numprocesses++;
      head = read_to_disk(time_cr, pr_id, mem_size, job_time, head);
    }
    fclose(file);
    return head;
}
void start_simulation(char *algoname, int mem_size, int quantum,
   int num_p, pronode_t *head){
  if(strcmp(algoname,FIRST) == 0){
    /*run first algo*/
  }
  else if(strcmp(algoname, BEST)==0){
    /*run best algo*/
  }
  else if(strcmp(algoname, WORST)==0){
    /*run worst algo*/
  }
  else{
    printf("Invalid Algo");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[]){
  /*Input File Variables*/
  int numprocesses = 0;
  pronode_t *head = NULL;

  /*Command Line Flag Variables*/
  int option;
  char *filename;
  char *algoname;
  int memsize;
  int quantum;

/*Commnad Line Flag Processing*/
  while ((option = getopt(argc, argv,"f:a:m:q:")) != -1) {
          switch (option) {
               case 'f' : filename = optarg;
                   break;
               case 'a' : algoname = optarg;
                   break;
               case 'm' : memsize = atoi(optarg);
                   break;
               case 'q' : quantum = atoi(optarg);
                   break;
               default: printf("Incorrect Input");
                   exit(EXIT_FAILURE);
          }
  }
  /*printf("%s, %s, %d, %d", filename, algoname, memsize, quantum);*/

  /*Read File Input*/
  head = read_from_file(filename, &numprocesses, head);
  start_simulation(algoname, memsize, quantum, numprocesses, head);
  print_disk(head);

  return 0;
}

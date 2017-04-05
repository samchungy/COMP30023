#include "functions.h"

disk_t * read_from_file(char *filename, int *numprocesses, disk_t *disk){
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
      disk->ready = create_process(time_cr, pr_id, mem_size, job_time,
        disk->ready);
    }
    fclose(file);
    return disk;
}
void start_simulation(char *algoname, int mem_size, int quantum,
   int num_p, pronode_t *head){
  int time = 0;
  int numswapped = 0;
  /*Initialise Memory*/
  list_t *memory = init_memory(mem_size);
  pronode_t *swapped = new_pronodelist();
  /*Assumes there will always be one process to start with*/
  memory = algo_select(head, algoname, memory);
  /*while loop*/
  while(1){
    process_t *popped;
    time++;

  }
  exit(EXIT_FAILURE);

  /*swap_process(proclist);*/

}

int main(int argc, char *argv[]){
  /*Input File Variables*/
  int numprocesses = 0;
  disk_t *disk = NULL;

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
  disk = read_from_file(filename, &numprocesses, head);
  print_disk(head);
  start_simulation(algoname, memsize, quantum, numprocesses, head);

  return 0;
}

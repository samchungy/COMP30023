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
  /*Initialise Memory*/
  list_t *memory;
  memory = malloc(sizeof(list_t));
  assert(memory != NULL);
  memory->data_free = mem_size;
  memory->pro_head = NULL;
  node_t *freemem = mem_size;
  freemem = malloc(sizeof(node_t));
  assert(freemem != NULL);
  free_head->start = STARTMEMORY;
  free_head->end = mem_size;
  free_head->size = mem_size;
  memory->free_head = freemem;

  /*Assumes there will always be one process to start with*/
  memory = algo_select(head.process, algoname, memory);
  /*while loop*/

  swap_process(proclist)

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

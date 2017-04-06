#include "functions.h"

disk_t * read_from_file(char *filename, disk_t *disk){
    int time_cr;
    int pr_id;
    int mem_size;
    int job_time;
    FILE *file = fopen (filename, "r");
    while(fscanf(file,"%d",&time_cr)!=EOF){
      fscanf(file,"%d",&pr_id);
      fscanf(file,"%d",&mem_size);
      fscanf(file,"%d",&job_time);
      disk->num_ready++;
      disk->ready = create_process(time_cr, pr_id, mem_size, job_time,
        disk->ready);
    }
    fclose(file);
    return disk;
}

void swap(mem_t **mem, process_t **temp, queue_t **scheduler, char **algoname,
disk_t **disk, int timer, int mem_size){
  (*temp) = pop_process(&(*disk), timer);
  if (*temp == NULL){
    printf("fuck offf\n");
  }
  fflush(stdout);
  (*mem) = insert_into_mem(*temp, *algoname,*mem,timer,&(*scheduler),&(*disk));
  (*scheduler) = insert_at_foot((*scheduler), *temp);
  printf("time %d, %d loaded, numprocesses=%d, numholes=%d, memusage=%d%%\n",
    timer, (*temp)->pr_id, (*mem)->numprocesses, (*mem)->numholes,
      (int)ceil((double)(mem_size-(*mem)->data_free)/(double)mem_size*100));
};

void schedule(process_t **CPU, queue_t **scheduler){
  (*CPU)=(*scheduler)->head->process;
}

void start_simulation(char *algoname, int mem_size, int quantum, disk_t *disk){
  /*Global Timer*/
  int timer = 0;
  int quant = quantum;
  /*CPU*/
  process_t *CPU = NULL;
  process_t *temp = NULL;
  queue_t *scheduler;
  scheduler = init_queue();
  /*Initialise Memory*/
  mem_t *memory = init_memory(mem_size);
  /*Assumes there will always be one process to start with*/
  swap(&memory, &temp, &scheduler, &algoname, &disk, timer, mem_size);
  schedule(&CPU, &scheduler);

  while(1){
    if (quant >= 0){
      if(CPU->run_time == CPU->job_time){
        /*Job Done - Remove Process, Reset & Swap*/
        CPU = NULL;
        free_pronode(pop_from_mem(&memory, pop_from_queue(&scheduler)));
        swap(&memory, &temp, &scheduler, &algoname, &disk, timer, mem_size);
        schedule(&CPU, &scheduler);
        quant = quantum;

      }
      else if (quant == 0){
        printf("pr_id: %d\n",temp->pr_id);
        fflush(stdout);
        /*Quant Over. Swap*/
        CPU = NULL;
        printf("pr_id: %d\n",temp->pr_id);
        fflush(stdout);
        scheduler = insert_at_foot(scheduler, pop_from_queue(&scheduler));
        printf("pr_id: %d\n",temp->pr_id);
        fflush(stdout);
        temp = NULL;
        swap(&memory, &temp, &scheduler, &algoname, &disk, timer, mem_size);
        schedule(&CPU, &scheduler);
        quant = quantum;
      }
      else{
        quant--;
        CPU->run_time++;
      }
    }
    printf("%d,%d\n",quant, temp->pr_id);
    timer++;
  }


}

int main(int argc, char *argv[]){
  /*Input File Variables*/
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
  disk = create_disk();
  disk = read_from_file(filename, disk);
  /*print_disk(disk);*/
  start_simulation(algoname, memsize, quantum, disk);

  return 0;
}

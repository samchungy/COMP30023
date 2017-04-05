#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#define FIRST "first"
#define BEST "best"
#define WORST "worst"
#define STARTMEMORY 1
#define TRUE 1

/*Process*/
typedef struct process_t{
  int time_cr;
  int pr_id;
  int mem_size;
  int job_time;
  int time_swapped;
  int startint;
  int endint;
}process_t;

/*Process Node for List*/
typedef struct pronode_t{
  process_t *process;
  struct pronode_t *next;
} pronode_t;

/*Disk Overarching Structure*/
typedef struct disk_t{
  pronode_t *ready;
  pronode_t *swap;
  int num_ready;
  int num_swap;
}disk_t;

/*Memory Node*/
typedef struct node_t{
  int size;
  int start;
  int end;
  struct node_t *next;
} node_t;

/*Main Memory Holder*/
typedef struct list_t{
  node_t *free_head;
  pronode_t *pro_head;
  int numprocesses;
  int data_free;
} list_t;

typedef struct queue_t{
  pronode_t *head;
  pronode_t *foot;
} queue_t;

/*-------CONSTRUCTOR FUNCTIONS-------*/
 /*Creates List of Process Nodes*/
 pronode_t *new_pronodelist();
 /*Creates Disk*/
 disk_t *create_disk();

/*-------DISK FUNCTIONS-------*/
/*Reads data, creates a new process and adds it to the linked list*/
pronode_t * create_process(int, int, int, int, pronode_t *);
/*Chooses & Pops out a process based on the spec rules**/
pronode_t * pop_process(disk_t **, int);
/*Changes the pronode list and pops out the process*/
pronode_t * pop_out_process(pronode_t **, int *num);


/*-------MEMORY FUNCTIONS-------*/
/*Initialises the memory struct*/
list_t * init_memory(int);
/*Selects the algo based on Specification*/
list_t * algo_select(pronode_t *, char *, list_t *);
/*Adds to the process list currently in memory*/
list_t * add_to_process_list(pronode_t *, list_t *);

/*Prints data out -- TEST FUNCTION*/
void print_disk(disk_t *);

/*--------ALGOS--------*/
/*Adds based on First Algorithm*/
list_t * add_first(pronode_t *, list_t *);

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <math.h>

#define FIRST "first"
#define BEST "best"
#define WORST "worst"
#define STARTMEMORY 1
#define NOTINMEM -1

/*Process*/
typedef struct process_t{
  int time_cr;
  int pr_id;
  int mem_size;
  int job_time;
  int time_swapped;
  int time_memoryin;
  int startint;
  int endint;
  int run_time;
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
typedef struct mem_t{
  node_t *free_head;
  pronode_t *pro_head;
  int data_free;
  int numholes;
} mem_t;

typedef struct queue_t{
  pronode_t *head;
  pronode_t *foot;
  int numitems;
} queue_t;

/*-------CONSTRUCTOR FUNCTIONS-------*/
 /*Creates Disk*/
 disk_t *create_disk();

/*-------DISK FUNCTIONS-------*/
/*Reads data, creates a new process and adds it to the linked list*/
pronode_t * create_process(int, int, int, int, pronode_t *);
/*Chooses & Pops out a process based on the spec rules**/
process_t * pop_process(disk_t **, int);
/*Changes the pronode list and pops out the process*/
process_t * pop_out_process(disk_t **, int *, int);
/*Adds process to swap space*/
void add_to_swapspace(disk_t **, process_t *, int);
/*Frees Pronode**/
void free_pronode(pronode_t *);
/*Pop out highest priority, longest in swap space*/
process_t *pop_longhigh_swap(disk_t **);
/*Gets Swap Space Highest Priority*/
int get_highest_pr_id(pronode_t *, int, int);

/*-------MEMORY FUNCTIONS-------*/
/*Initialises the memory struct*/
mem_t * init_memory(int);
/*Inserts Process Into Memory*/
mem_t * insert_into_mem(process_t *, char *, mem_t *, int, queue_t **, disk_t **);
/*Adds to the process list currently in memory*/
mem_t * add_to_process_list(pronode_t *, mem_t *);
/*Takes away free Space & assigns process memory ints*/
void assign_to_memory(process_t **, node_t **, node_t **, mem_t **, int);
/*Removeds Process from Memory*/
process_t * pop_from_mem(mem_t **, process_t *);
/*Pop out process which has been in memory the longest*/
process_t * pop_out_longest_in_mem(mem_t **);
/*Creates a new memory node*/
node_t *new_mem_node(int, int, int, node_t *);
/*in-order insert of free space into free list, merges blocks together*/
mem_t *restore_free_space(mem_t *, int, int, int);
/*free node*/
void free_node(node_t *);
/*New Pronode*/
pronode_t *new_pronode(process_t *pro);

/*--------ALGOS--------*/
/*Adds based on First Algorithm*/
mem_t * add_first(pronode_t *, mem_t *, int);
/*Adds based on Best Algorithm*/
mem_t * add_best(pronode_t *, mem_t *, int);
/*Adds based on Worst Algorithm*/
mem_t * add_worst(pronode_t *, mem_t *, int);


/*------QUEUE FUNCTIONS--------*/
/*Initialises Queue*/
queue_t *init_queue();
/*Pop from queue (Selective)*/
process_t *pop_from_queue_select(queue_t **, process_t *);
/*Pop Head of Queue*/
process_t *pop_from_queue(queue_t **);
/*Insert at Foot of Queue*/
queue_t *insert_at_foot(queue_t *, process_t *);

/*Prints data out -- TEST FUNCTION*/
void print_disk(disk_t *);
void printqueue(queue_t *, int);
void printswap(disk_t *);
void printdatachunks(mem_t *);

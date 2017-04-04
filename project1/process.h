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

typedef struct process_t{
  int time_cr;
  int pr_id;
  int mem_size;
  int job_time;
  int time_swapped;
  bool indisk;
}process_t;

typedef struct pronode_t{
  process_t process;
  struct pronode_t *next;
} pronode_t;

/*Reads data, creates a new process and adds it to a linked list*/
pronode_t * read_to_disk(int, int, int, int, pronode_t *);
/*Prints data out -- TEST FUNCTION*/
void print_disk(pronode_t *);

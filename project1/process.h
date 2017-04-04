#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


typedef struct process_t{
  int time_cr;
  int pr_id;
  int mem_size;
  int job_time;
}process_t;

typedef struct pronode_t{
  process_t process;
  struct pronode_t *next;
} pronode_t;

typedef struct list_t{
  node_t start;
  int data_free;
} list_t;

typedef struct node_t{
  pronode_t process;
  int start;
  int end;
  node_t next;
} node_t;

pronode_t * read_to_disk(int, int, int, int, pronode_t *);
void print_disk(pronode_t *);

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct process_t{
  int time_cr;
  int pr_id;
  int mem_size;
  int job_time;
}process_t;

typedef struct node_t{
  process_t process;
  struct node_t *next;
} node_t;

node_t * read_to_disk(int, int, int, int, node_t *);

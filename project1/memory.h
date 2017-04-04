#include "process.h"

/*Main Memory Holder*/
typedef struct list_t{
  node_t *free_head;
  pronode_t *pro_head;
  int data_free;
} list_t;

/*Memory Node*/
typedef struct node_t{
  int size;
  int start;
  int end;
  node_t *next;
} node_t;

list_t * algo_select(pronode_t *, char *, list_t *);

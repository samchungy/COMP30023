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

/*Initialises the memory struct*/
list_t * init_memory(int);
/*Selects the algo based on Specification*/
list_t * algo_select(process_t *, char *, list_t *);
/*Adds to the process list currently in memory*/
list_t * add_to_process_list(pronode_t *, list_t *);

/*ALGOS*/
/*Adds based on First Algorithm*/
list_t * add_first(process_t *, list_t *);

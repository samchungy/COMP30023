#include "process.h"

list_t * init_memory(int mem_size){
  /*New Memory Struct*/
  list_t *new;
  new = malloc(sizeof(list_t));
  assert(new != NULL);

  new->data_free = mem_size;
  new->pro_head = NULL;
  /*New Free List Struct*/
  node_t *freemem;
  freemem = malloc(sizeof(node_t));
  assert(freemem != NULL);

  freemem->start = STARTMEMORY;
  freemem->end = mem_size;
  freemem->size = mem_size;
  new->free_head = freemem;
  return new;
}

list_t * algo_select(process_t *proc, char *algoname, list_t *memory){
  int freemem = memory->data_free;
  if(strcmp(algoname, FIRST) == 0){
    /*memory = add_first(proc, memory);*/
  }
  else if(strcmp(algoname, BEST)==0){
    /*run best algo*/
  }
  else if(strcmp(algoname, WORST)==0){
    /*run worst algo*/
  }
  else{
    printf("Invalid Algo");
    exit(EXIT_FAILURE);
  }
  if (freemem == memory->data_free){
    /*Failed to place process in memory state*/
    return memory;
  }
  else{
    return memory;
  }
  /*Satisfy compiler errors*/
  return NULL;
}

list_t * add_first(process_t *proc, list_t *memory){
  if (memory->data_free<proc->mem_size){
    /*Definitely no memory slots available*/
    return memory;
  }

node_t *curr = memory->free_head;
pronode_t *procurr = memory->pro_head;

/*Iterate over Free Lists*/
while(curr != NULL){
  if (proc->mem_size < curr->size){
    /*Found memory space! Add to running processes list*/
    while(procurr->next!= NULL){
      procurr = procurr->next;
    }
    /*Add Process Start/End Address, Modify Free Space Item*/
    proc->startint = curr->start;
    proc->endint = proc->startint + ((proc->mem_size)-1);
    curr->start = proc->endint;
    curr->end = curr->start + (curr->size-((proc->mem_size)-1));
    memory->data_free -= proc->mem_size;
    return add_to_process_list(proc, memory);
  }
  else{
    /*Memory space is too small*/
    curr = curr->next;
  }
}
/*Fail State - No Memory Spots*/
return memory;

}

list_t * add_to_process_list(process_t *proc, list_t *memory){
  pronode_t *new;
  new = malloc(sizeof(pronode_t));
  assert(new != NULL);
  new->process = proc;
  new->next = NULL;

  pronode_t *curr = memory->pro_head;
  while(curr->next!=NULL){
    curr=curr->next;
  }
  curr->next = new;
  return memory;
}

#include "functions.h"

mem_t * init_memory(int mem_size){
  /*New Memory Struct*/
  mem_t *new;
  new = malloc(sizeof(*new));
  assert(new != NULL);

  new->data_free = mem_size;
  new->pro_head = NULL;
  new->numprocesses = 0;
  /*New Free List Struct*/
  node_t *freemem;
  freemem = malloc(sizeof(*freemem));
  assert(freemem != NULL);

  freemem->start = STARTMEMORY;
  freemem->end = mem_size;
  freemem->size = mem_size;
  freemem->next = NULL;
  new->free_head = freemem;
  return new;
}

mem_t * algo_select(pronode_t *proc, char *algoname, mem_t *memory){
  int freemem = memory->data_free;
  if(strcmp(algoname, FIRST) == 0){
    memory = add_first(proc, memory);\
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

mem_t * add_first(pronode_t *proc, mem_t *memory){
  if (memory->data_free<proc->process->mem_size){
    /*Definitely no memory slots available*/
    return memory;
  }

  node_t *curr = memory->free_head;
  /*Iterate over Free Lists*/
  while(curr != NULL){
    if (proc->process->mem_size < curr->size){
      /*Found memory space! - Modify Free Space, Add Space Spec to Process*/
      proc->process->startint = curr->start;
      proc->process->endint = proc->process->startint + ((proc->process->mem_size)-1);
      curr->start = proc->process->endint;
      curr->end = curr->start + (curr->size-((proc->process->mem_size)-1));
      memory->data_free -= proc->process->mem_size;
      memory->numprocesses++;
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

mem_t * add_to_process_list(pronode_t *proc, mem_t *memory){
  pronode_t *curr = memory->pro_head;
  if (curr == NULL){
    memory->pro_head = proc;
    return memory;
  }
  while(curr->next!=NULL){
    curr=curr->next;
  }
  curr->next = proc;
  return memory;
}

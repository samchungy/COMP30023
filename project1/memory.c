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
  if (freemem<proc->process->mem_size){
    /*Definitely no memory slots available*/
    return memory;
  }
  if(strcmp(algoname, FIRST) == 0){
    /*run first algo*/
    memory = add_first(proc, memory);
  }
  else if(strcmp(algoname, BEST)==0){
    /*run best algo*/
    memory = add_best(proc, memory);
  }
  else if(strcmp(algoname, WORST)==0){
    /*run worst algo*/
    memory = add_worst(proc, memory);
  }
  else{
    printf("Invalid Algo");
    exit(EXIT_FAILURE);
  }
  return memory;
}

mem_t * add_first(pronode_t *proc, mem_t *memory){
  node_t *curr = memory->free_head;
  node_t *prev = memory->free_head;
  /*Iterate over Free Lists*/
  while(curr != NULL){
    if (proc->process->mem_size <= curr->size){
      /*Found memory space! - Modify Free Space, Add Space Spec to Process*/
      assign_to_memory(&(proc->process),&curr, &prev, &memory);
      return add_to_process_list(proc, memory);
    }
    else{
      /*Memory space is too small*/
      prev = curr;
      curr = curr->next;
    }
  }
  /*Fail State - No Memory Spots*/
  return memory;

}

mem_t * add_best(pronode_t *proc, mem_t *memory){
  node_t *curr = memory->free_head;
  node_t *prev = memory->free_head;
  node_t *best = NULL;
  node_t *bestprev;
  /*Iterate over Free List*/
  while(curr!=NULL){
    if (proc->process->mem_size <= curr->size){
      /*Foud a data spot which fits*/
      if(best==NULL || curr->size - proc->process->mem_size <=
        best->size - proc->process->mem_size){
        /*First Case or Item is Smaller/Equal to Best*/
        best = curr;
        bestprev = prev;
      }
      else{
        prev = curr;
        curr = curr->next;
      }
    }
    else{
      prev = curr;
      curr = curr->next;
    }
  }

  if(best == NULL){
    /*No Memory Cell Fits*/
    return memory;
  }
  else{
    /*Found one which fits*/
    assign_to_memory(&(proc->process),&best, &bestprev, &memory);
    return add_to_process_list(proc, memory);
  }
}

mem_t *add_worst(pronode_t *proc, mem_t *memory){
  node_t *curr = memory->free_head;
  node_t *prev = memory->free_head;
  node_t *largest = NULL;
  node_t *largestprev;

  /*Iterate over Free List*/
  while(curr!=NULL){
    if (proc->process->mem_size <= curr->size){
      /*Foud a data spot which fits*/
      if(largest==NULL || curr->size - proc->process->mem_size >=
        largest->size - proc->process->mem_size){
        /*First Case or Item is Smaller/Equal to Best*/
        largest = curr;
        largestprev = prev;
      }
      else{
        prev = curr;
        curr = curr->next;
      }
    }
    else{
      prev = curr;
      curr = curr->next;
    }
  }

  if(largest == NULL){
    /*No Memory Cell Fits*/
    return memory;
  }
  else{
    /*Found one which fits*/
    assign_to_memory(&(proc->process),&largest, &largestprev, &memory);
    return add_to_process_list(proc, memory);
  }
}

void assign_to_memory(process_t **proc, node_t **node, node_t **prev,
   mem_t **mem){
  (*proc)->startint = ((*node)->end)+1-(*proc)->mem_size;
  (*proc)->endint = ((*node)->end);
  (*node)->end = (*proc)->startint-1;
  (*node)->size -= (*proc)->mem_size;
  (*memory)->data_free -= (*proc)->mem_size;
  (*memory)->numprocesses++;
  if ((*node)->size == 0){
    (*prev)->next = (*node)->next;
    free_node((*node));
  }
}

void free_node(node_t *node){
  assert(node != NULL);
  free(node);
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

#include "functions.h"

node_t *new_mem_node(int mem, int start, int end, node_t *next){
  node_t *new;
  new = malloc(sizeof(*new));
  assert(new!=NULL);
  new->start = start;
  new->size = mem;
  new->end = end;
  new->next=next;
  return new;
}

mem_t * init_memory(int mem_size){
  /*New Memory Struct*/
  mem_t *new;
  new = malloc(sizeof(*new));
  assert(new != NULL);

  new->data_free = mem_size;
  new->pro_head = NULL;
  new->numholes =1;
  /*New Free List Struct*/
  new->free_head = new_mem_node(mem_size, STARTMEMORY, mem_size, NULL);
  return new;
}

process_t * pop_out_longest_in_mem(mem_t **mem){
  pronode_t *curr = (*mem)->pro_head;
  assert(curr != NULL);
  pronode_t *prev = NULL;
  pronode_t *longest = NULL;
  pronode_t *longestprev = NULL;

  if(curr->next == NULL){
    (*mem) = restore_free_space(*mem, curr->process->startint,
      curr->process->endint, curr->process->mem_size);
    (*mem)->pro_head=NULL;
    process_t *temp;
    temp = curr->process;
    free_pronode(curr);
    return temp;
  }

  while(curr != NULL){
    if(longest == NULL || curr->process->time_memoryin <
      longest->process->time_memoryin){
        longest = curr;
        longestprev = prev;
      }
    prev = curr;
    curr = curr->next;
  }

  if (longestprev == NULL){
    (*mem)->pro_head = longest->next;
  }
  else{
    longestprev->next = longest->next;
  }

  (*mem) = restore_free_space(*mem, longest->process->startint,
    longest->process->endint, longest->process->mem_size);
    process_t *temp = longest->process;
    free_pronode(longest);
  return temp;
}

mem_t * insert_into_mem(process_t *process, char *algoname, mem_t *memory,
  int timer, queue_t **queue, disk_t **disk){
  pronode_t *proc = new_pronode(process);
  int triedtoinsert = 0;
  while(proc->process->time_memoryin == NOTINMEM){
    int freemem = memory->data_free;
    if (!triedtoinsert){
      if(strcmp(algoname, FIRST) == 0){
        /*run first algo*/
        memory = add_first(proc, memory, timer);
      }
      else if(strcmp(algoname, BEST)==0){
        /*run best algo*/
        memory = add_best(proc, memory, timer);
      }
      else if(strcmp(algoname, WORST)==0){
        /*run worst algo*/
        memory = add_worst(proc, memory, timer);
      }
      if (freemem == memory->data_free){
        /*Insert Failed*/
        triedtoinsert = 1;
      }
      else{
        /*Insert Succeeded*/
        return memory;
      }
    }
    else{
      /*Make Space*/
      printf("NOT ENOUGH SPACE\n");
      process_t *procc = pop_out_longest_in_mem(&memory);
      printf("%d POPPED OUT - Space: %d-%d\n", procc->pr_id, procc->startint, procc->endint);
      procc->time_memoryin = NOTINMEM;
      pop_from_queue_select(&(*queue),procc);
      add_to_swapspace(&(*disk),procc, timer);
      triedtoinsert = 0;
    }
  }

  return memory;
}

mem_t * add_first(pronode_t *proc, mem_t *memory, int timer){
  node_t *curr = memory->free_head;
  node_t *prev = memory->free_head;
  /*Iterate over Free Lists*/
  while(curr != NULL){
    if (proc->process->mem_size <= curr->size){
      /*Found memory space! - Modify Free Space, Add Space Spec to Process*/
      assign_to_memory(&(proc->process),&curr, &prev, &memory, timer);
      return add_to_process_list(proc, memory);
    }
    prev = curr;
    curr = curr->next;
  }
  /*Fail State - No Memory Spots*/
  return memory;
}

mem_t * add_best(pronode_t *proc, mem_t *memory, int timer){
  node_t *curr = memory->free_head;
  node_t *prev = NULL;
  node_t *best = NULL;
  node_t *bestprev = NULL;

  if (curr->next == NULL){
    if (proc->process->mem_size <= curr->size){
      printf("One cell avail\n");
      /*Only 1 cell available and of the correct size*/
      assign_to_memory(&(proc->process),&curr, &prev, &memory, timer);
      return add_to_process_list(proc, memory);
    }
    else{
      /*Memory not big enough*/
      printf("POOP\n");
      return memory;
    }
  }

  while(curr!=NULL){
    if(proc->process->mem_size <= curr->size){
      /*found a data spot which is of the right size*/
      if(best == NULL || curr->size-proc->process->mem_size <
        best->size - proc->process->mem_size){
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
    /*No Space Found*/
    return memory;
  }
  if (bestprev == NULL){
    memory->free_head = best->next;
    assign_to_memory(&(proc->process),&best, &bestprev, &memory, timer);
    return add_to_process_list(proc, memory);
  }
  assign_to_memory(&(proc->process),&best, &bestprev, &memory, timer);
  return add_to_process_list(proc, memory);
}


mem_t *add_worst(pronode_t *proc, mem_t *memory, int timer){
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
    assign_to_memory(&(proc->process),&largest, &largestprev,&memory,timer);
    return add_to_process_list(proc, memory);
  }
}

void assign_to_memory(process_t **proc, node_t **node, node_t **prev,
   mem_t **mem, int timer){
  (*proc)->startint = (*node)->start;
  (*proc)->endint = ((*node)->start)+(*proc)->mem_size-1;
  (*proc)->time_memoryin = timer;
  (*node)->start = (*proc)->endint+1;
  (*node)->size -= (*proc)->mem_size;
  (*mem)->data_free -= (*proc)->mem_size;
  printf("Assigned %d - %d\n", (*proc)->startint, (*proc)->endint);
  printf("Data Free %d\n", (*node)->start);
  if ((*node)->size == 0){
    printf("KILLED\n");
    if((*prev)!=NULL){
      (*prev)->next = (*node)->next;
    }
    free_node((*node));
  }
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


void free_node(node_t *node){
  assert(node != NULL);
  free(node);
}

process_t * pop_from_mem(mem_t **mem, process_t *proc){
  pronode_t *curr = (*mem)->pro_head;
  pronode_t *prev = NULL;
  process_t *temp;
  if (curr->next == NULL){
    /*1 item only*/
    (*mem) = restore_free_space(*mem, curr->process->startint,
      curr->process->endint, curr->process->mem_size);
      temp = curr->process;
      (*mem)->pro_head = NULL;
      free_pronode(curr);
      return temp;
  }
  while(curr->process->pr_id != proc->pr_id){
    prev = curr;
    curr = curr->next;
  }

  if(prev == NULL){
    (*mem)->pro_head = curr->next;

    (*mem) = restore_free_space(*mem, curr->process->startint,
    curr->process->endint, curr->process->mem_size);
    temp = curr->process;
    free_pronode(curr);
    return temp;
  }

  prev->next = curr->next;

  (*mem) = restore_free_space(*mem, curr->process->startint,
    curr->process->endint, curr->process->mem_size);
    temp = curr->process;
    free_pronode(curr);
    return temp;
}

mem_t *restore_free_space(mem_t *mem, int start, int end, int size){
  node_t *curr = mem->free_head;
  mem->data_free+=size;
  if (curr != NULL){
    while(curr!=NULL){
        if (end < curr->start){
          /*Free Space has a smaller address than first node*/
          if (end+1 == curr->start){
            /**Next Space is adjacent. Add free space to that node.*/
            curr->start = start;
            curr->size+=size;
            return mem;
          }
          else{
            /**Create a new node, the next data node does not connect*/
            mem->free_head = new_mem_node(size, start, end, curr);
            mem->numholes++;
            return mem;
          }
        }
        else{
          /*Free Space Node has a lower value than freed space*/
          if (curr->end == start-1){
            /*Free Space connects with Free Space Node*/
            if (curr->next == NULL || !(curr->next->start==end+1) ){
              /*There is no other free space connected or the next space
              does not connect up, combine spaces*/
              curr->end = end;
              curr->size+=size;
              return mem;
            }
            else{
              /*Next available space connects with this one Merge all 3*/
              curr->size+=(curr->next->size+size);
              curr->end = curr->next->end;
              node_t *temp = curr->next->next;
              free_node(curr->next);
              mem->numholes--;
              curr->next = temp;
              return mem;
            }
          }
          else{
            /*Not Connected to current Node*/
            if (curr->next == NULL){
              /*No data available so create a new node*/
              curr->next = new_mem_node(size, start, end, NULL);
              mem->numholes++;
              return mem;
            }
            else if (!(curr->next->start <= end+1)){
              /*Another Node Available, it's larger than free space address*/
              curr->next = new_mem_node(size, start, end, curr->next);
              mem->numholes++;
              return mem;
            }
            else{
              /*Next node is smaller. Let the while loop deal with this*/
            }
          }
        }
        curr = curr->next;
    }
    return mem;
  }
  else{
    /*Free List is Empty - Create a new node*/
    mem->free_head = new_mem_node(size, start, end, NULL);
    mem->numholes++;
    return mem;
  }
}

void printdatachunks(mem_t *mem){
  node_t *curr = mem->free_head;
  while(curr != NULL){
    printf("%d to %d free SIZE: %d\n", curr->start, curr->end, curr->size);
    curr = curr->next;
  }
}

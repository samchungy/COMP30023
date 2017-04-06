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
  new->numprocesses = 0;
  /*New Free List Struct*/
  new->free_head = new_mem_node(mem_size, STARTMEMORY, mem_size, NULL);
  return new;
}

pronode_t * pop_out_longest_in_mem(mem_t **mem){
  pronode_t *curr = (*mem)->pro_head;
  pronode_t *prev = NULL;
  pronode_t *longest = NULL;
  pronode_t *longestprev = NULL;
  if(curr->next == NULL){
    (*mem) = restore_free_space(*mem, curr->process->startint,
      curr->process->endint, curr->process->mem_size);
    (*mem)->pro_head=NULL;
    return curr;
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
  longestprev->next = longest->next;
  longest->next = NULL;
  (*mem) = restore_free_space(*mem, longest->process->startint,
    longest->process->endint, longest->process->mem_size);
  return longest;

}

mem_t * insert_into_mem(pronode_t *proc, char *algoname, mem_t *memory,
  int timer, queue_t **queue, disk_t **disk){
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
      if (freemem != memory->data_free){
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
      pronode_t *pronod = pop_out_longest_in_mem(&memory);
      pop_from_queue_select(&(*queue),pronod);
      add_to_swapspace(&(*disk),pronod);
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
    else{
      /*Memory space is too small*/
      prev = curr;
      curr = curr->next;
    }
  }
  /*Fail State - No Memory Spots*/
  return memory;

}

mem_t * add_best(pronode_t *proc, mem_t *memory, int timer){
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
    assign_to_memory(&(proc->process),&best, &bestprev, &memory, timer);
    return add_to_process_list(proc, memory);
  }
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
  (*proc)->startint = ((*node)->end)+1-(*proc)->mem_size;
  (*proc)->endint = ((*node)->end);
  (*proc)->time_memoryin = timer;
  (*node)->end = (*proc)->startint-1;
  (*node)->size -= (*proc)->mem_size;
  (*mem)->data_free -= (*proc)->mem_size;
  (*mem)->numprocesses++;
  if ((*node)->size == 0){
    (*prev)->next = (*node)->next;
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

pronode_t * pop_from_mem(mem_t **mem, pronode_t *proc){
  pronode_t *curr = (*mem)->pro_head;
  pronode_t *prev = (*mem)->pro_head;
  while(curr->process->pr_id != proc->process->pr_id){
    prev = curr;
    curr = curr->next;
  }
  prev->next = curr->next;
  curr->next = NULL;
  (*mem) = restore_free_space(*mem, curr->process->startint,
    curr->process->endint, curr->process->mem_size);
  return curr;
}

mem_t *restore_free_space(mem_t *mem, int start, int end, int size){
  node_t *curr = mem->free_head;
  mem->numprocesses--;
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
              curr->next = temp;
              return mem;
            }
          }
          else{
            /*Not Connected to current Node*/
            if (curr->next == NULL){
              /*No data available so create a new node*/
              curr->next = new_mem_node(size, start, end, NULL);
              return mem;
            }
            else if (!(curr->next->start <= end+1)){
              /*Another Node Available, it's larger than free space address*/
              curr->next = new_mem_node(size, start, end, curr->next);
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
    return mem;
  }
}

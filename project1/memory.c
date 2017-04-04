#include "memory.h"
#include "process.h"

list_t * algo_select(pronode_t *proc, char *algoname, list_t *memory){
  if(strcmp(algoname, FIRST) == 0){
    return(add_first(proc, algoname, memory));
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
}

list_t * add_first(pronode_t *proc, char *algoname, list_t *memory){
  if (memory.data_free<proc.mem_size){
    /*Shortcut to no data free*/
    /*set fail status*/
    return memory;
  }
node_t *new;
new = malloc(sizeof(node_t));
assert(new!=NULL);
new.size = proc.mem_size;
new.next = NULL;
node_t *curr = memory.free_head;

while(curr != NULL){
  if (new.size < curr.size){
    /*Found memory space!*/
    curr.
  }
  else{
    /*Memory space is too small*/
    curr = curr->next;
  }
}
/*Fail Status*/
return memory;

}

node_t * add_proccess_to_memory(node_t *new, node_t *curr){
  new.start = curr.start;
  new.end = new.start + (new.size-1);
  curr.start = new.end;
  curr.end = curr.start + (curr.size-new.size-1);

}

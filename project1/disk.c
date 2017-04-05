  #include "functions.h"

  pronode_t * create_process(int time_cr, int pr_id, int mem_size, int job_time,
    pronode_t *ready){

    pronode_t *newpronode;
    newpronode = malloc(sizeof(*newpronode));
    assert(newpronode!=NULL);

    process_t *newproc;
    newproc = malloc(sizeof(*newproc));
    assert(newproc != NULL);

    newproc->time_cr = time_cr;
    newproc->pr_id = pr_id;
    newproc->mem_size = mem_size;
    newproc->job_time = job_time;
    newpronode->process = newproc;
    newpronode->next=NULL;

    if (ready->process==NULL){
      /*New List*/
      return newpronode;
    }
    else{
      /*New Node for List*/
      pronode_t *curr;
      curr = ready;
      while(curr->next != NULL){
        curr = curr->next;
      }
      curr->next = newpronode;
      return ready;
    }

}

disk_t * create_disk(){
  /**Initialise Disk**/
  disk_t *disk;
  pronode_t *ready;
  pronode_t *swap;

  disk = malloc(sizeof(*disk));
  assert(disk!=NULL);
  ready = new_pronodelist();
  swap = new_pronodelist();
  disk->ready = ready;
  disk->swap = swap;
  disk->num_ready = 0;
  disk->num_swap = 0;
  return disk;
}

pronode_t *new_pronodelist(){
  pronode_t *new;
  new = malloc(sizeof(*new));
  assert(new != NULL);
  new->process = NULL;
  new->next=NULL;
  return new;
}

void print_disk(disk_t *disk){
  pronode_t *curr;
  curr = disk->ready;
  while(curr != NULL){
    printf("%d, %d, %d, %d\n",curr->process->time_cr, curr->process->pr_id,
      curr->process->mem_size, curr->process->job_time);
      curr = curr->next;
  }
}

pronode_t *pop_process(disk_t **disk, int timer){
  process_t *ready = (*disk)->ready->process;
  process_t *swap = (*disk)->swap->process;

  if(!(*disk)->num_swap){
    if(!(*disk)->num_ready){
      /*No Processes in Disk or Swapped Space*/
      return NULL;
    }
    else{
      /*Get process from the disk*/
      if(ready->time_cr >= timer){
        return pop_out_process(&(*disk)->ready, &(*disk)->num_ready);
      }
      else{
        /*No process created or available*/
        return NULL;
      }
    }
  }
  else{
    if(!(*disk)->num_ready){
      /*No Process to be created so swap in swap space process*/
      return pop_out_process(&(*disk)->swap, &(*disk)->num_swap);
    }
    else{
      /*Need to compare disk times*/
      if(ready->time_cr >= timer){
        if ((timer-ready->time_cr > timer-swap->time_swapped) ||
          ((timer-ready->time_cr == timer-swap->time_swapped)
            && (ready->pr_id <= swap->pr_id))){
          /*Created Process has been sitting longer than swapped process
          or they're the same and created process' p_id is higher priority*/
          return pop_out_process(&(*disk)->ready, &(*disk)->num_ready);
        }
      }
      return pop_out_process(&(*disk)->swap, &(*disk)->num_swap);
    }
  }
  /*Satisfy Compiler*/
  return NULL;
}

pronode_t *pop_out_process(pronode_t **list, int *num){
  pronode_t *temp = *list;
  *list = temp->next;
  (*num)--;
  temp->next = NULL;
  return temp;
}
/*
void swap_process(pronode_t *head, char *algoname){
  pronode_t *curr = head->next;
  pronode_t *highest = head;
  while(curr != NULL){
    if (highest->process.time_cr < curr->process.time_cr ||
      (highest->process.time_cr == curr->process.time_cr &&
        highest->process.pr_id > curr->process.pr_id)){
          highest = curr;
    }
  }
}
*/

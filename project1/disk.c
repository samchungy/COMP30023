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
    newproc->run_time = 0;
    newproc->time_memoryin = NOTINMEM;
    newpronode->process = newproc;
    newpronode->next=NULL;

    if (ready==NULL){
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
  pronode_t *ready = NULL;
  pronode_t *swap = NULL;

  disk = malloc(sizeof(*disk));
  assert(disk!=NULL);
  disk->ready = ready;
  disk->swap = swap;
  disk->num_ready = 0;
  disk->num_swap = 0;
  return disk;
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

pronode_t *new_pronode(process_t *pro){
  pronode_t *new;
  new = malloc(sizeof(*new));
  assert(new!=NULL);
  new->process = pro;
  new->next = NULL;
  return new;
}

void printswap(disk_t *disk){
  pronode_t *curr = disk->swap;
  while(curr != NULL){
    printf("Swap Space: Process - %d\n", curr->process->pr_id);
    curr = curr->next;
  }
}

process_t *pop_longhigh_swap(disk_t **disk){
  assert((*disk)->swap !=NULL);
  pronode_t *curr = (*disk)->swap;
  pronode_t *prev = NULL;
  pronode_t *best = NULL;
  pronode_t *bestprev = NULL;
  process_t *temp;

  if(curr->next == NULL){
    /*Only 1 thing in swapspace*/
    temp = curr->process;
    (*disk)->num_swap--;
    free_pronode(curr);
    (*disk)->swap = NULL;
    return temp;
  }
  prev = curr;
  curr = curr->next;

  while(curr != NULL){
    if(best == NULL || curr->process->time_swapped <
      best->process->time_swapped){
      best = curr;
      bestprev = prev;
    }
    else if (curr->process->time_swapped == best->process->time_swapped &&
    curr->process->pr_id < best->process->pr_id){
      best = curr;
      bestprev = prev;
    }
    prev = curr;
    curr = curr->next;
  }
  temp = best->process;
  bestprev->next = best->next;
  (*disk)->num_swap--;
  free_pronode(best);
  return temp;
}

process_t *pop_process(disk_t **disk, int timer){
  pronode_t *ready = (*disk)->ready;
  pronode_t *swap = (*disk)->swap;

  if((*disk)->num_swap){
    //printswap(*disk);
    printf("Fuckers in Swap Space\n");
    /*If there are processes in the swap space*/
    if((*disk)->num_ready && timer-ready->process->time_cr >= 0){
      printf("%d",timer-swap->process->time_swapped);
      /*If there are processes in both ready space and swap space*/
      /*COMPARE*/
      if(timer-ready->process->time_cr > timer-swap->process->time_swapped){
        /*If the ready process has been on disk longer than the swapped process*/
        printf("FELL HERE\n");
        return pop_out_process(&(*disk)->ready, &(*disk)->num_ready);
      }
      else if(timer-ready->process->time_cr ==
        timer-swap->process->time_swapped){
          printf("OR HERE\n");
          /*If they are the same age - compare pids*/
          if(ready->process->pr_id<swap->process->pr_id){
            return pop_out_process(&(*disk)->ready, &(*disk)->num_ready);
          }
          else{
            return pop_longhigh_swap(&(*disk));
          }
      }
      else{
        printf("HERE 1\n");
        /*Swapped Process is older*/
        return pop_longhigh_swap(&(*disk));
      }
    }
    else{
      /*There are only Swap Space Processes*/
      return pop_longhigh_swap(&(*disk));
    }
  }
  else{
    if((*disk)->num_ready && timer-ready->process->time_cr >= 0){
      return pop_out_process(&(*disk)->ready, &(*disk)->num_ready);
    }
    else{
      /*Nothing available to return right now*/
      return NULL;
    }
  }
}

process_t *pop_out_process(pronode_t **list, int *num){
  pronode_t *temp = *list;
  process_t *temper;
  (*list) = temp->next;
  (*num)--;
  temper = temp->process;
  free_pronode(temp);
  return temper;
}

void free_pronode(pronode_t *pro){
  assert(pro != NULL && pro->process != NULL);
  free(pro);
}

void add_to_swapspace(disk_t **disk, process_t *process, int timer){
  process->time_swapped = timer;
  if((*disk)->swap == NULL){
    (*disk)->swap = new_pronode(process);
    (*disk)->num_swap++;
    printf("NEW SWAP SAPCE: %d\n", (*disk)->swap->process->pr_id);
    return;
  }
  printf("Added to Swap\n");
  pronode_t *curr = (*disk)->swap;
  while(curr->next!= NULL){
    curr = curr->next;
  }
  curr->next = new_pronode(process);
  (*disk)->num_swap++;
  //printswap(*disk);
}

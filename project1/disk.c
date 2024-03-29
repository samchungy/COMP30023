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
    //printf("Swap Space: Process - %d\n", curr->process->pr_id);
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
  best = curr;
  prev = curr;
  curr = curr->next;

  while(curr != NULL){
    if(curr->process->time_swapped < best->process->time_swapped){
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
  if (bestprev == NULL){
    temp = best->process;
    (*disk)->num_swap--;
    (*disk)->swap = (*disk)->swap->next;
    free_pronode(best);
    return temp;
  }

  temp = best->process;
  bestprev->next = best->next;
  (*disk)->num_swap--;
  free_pronode(best);
  return temp;
}

int get_highest_pr_id(pronode_t *pn, int timeondisk, int timer){
  assert(pn != NULL);
  pronode_t *curr = pn;
  int highestpr = curr->process->pr_id;
  while(curr != NULL){
    if(curr->process->pr_id < highestpr
    && timeondisk == timer-curr->process->time_swapped){
      highestpr = curr->process->pr_id;
    }
    curr=curr->next;
  }
  return highestpr;
}

process_t *pop_process(disk_t **disk, int timer){
  pronode_t *ready = (*disk)->ready;
  pronode_t *swap = (*disk)->swap;

  if((*disk)->num_swap){
    printswap(*disk);
    /*If there are processes in the swap space*/
    if((*disk)->num_ready && timer-ready->process->time_cr >= 0){
      /*If there are processes in both ready space and swap space*/
      /*COMPARE*/
      if(timer-ready->process->time_cr > timer-swap->process->time_swapped){
        /*If the ready process has been on disk longer than the swapped process*/
        return pop_out_process(&(*disk), &(*disk)->num_ready, timer);
      }
      else if(timer-ready->process->time_cr ==
        timer-swap->process->time_swapped){
          /*If they are the same age - compare pids*/
          if(ready->process->pr_id<get_highest_pr_id(swap,
            timer-ready->process->time_cr, timer)){
            return pop_out_process(&(*disk), &(*disk)->num_ready, timer);
          }
          else{
            return pop_longhigh_swap(&(*disk));
          }
      }
      else{
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
      return pop_out_process(&(*disk), &(*disk)->num_ready, timer);
    }
    else{
      /*Nothing available to return right now*/
      return NULL;
    }
  }
}

process_t *pop_out_process(disk_t **disk, int *num, int timer){
  process_t *temper;
  pronode_t *curr = (*disk)->ready;
  pronode_t *prev = NULL;
  pronode_t *best = (*disk)->ready;
  pronode_t *bestprev = NULL;
  int benchmark = timer-best->process->time_cr;

  while(curr != NULL){
    if (timer-curr->process->time_cr != benchmark){
      break;
    }
    else if (timer-curr->process->time_cr >= 0 &&
      curr->process->pr_id < best->process->pr_id){
      best = curr;
      bestprev = prev;
    }
    prev = curr;
    curr = curr->next;
  }

  if(bestprev == NULL){
    /*First Item*/
    temper = best->process;
    (*disk)->ready = best->next;
    (*num)--;
    free_pronode(best);
    return temper;
    }
  else{
    temper = best->process;
    bestprev->next = best->next;
    (*num)--;
    free_pronode(best);
    return temper;
  }

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
    return;
  }
  pronode_t *curr = (*disk)->swap;
  while(curr->next!= NULL){
    curr = curr->next;
  }
  curr->next = new_pronode(process);
  (*disk)->num_swap++;
  printswap(*disk);
}

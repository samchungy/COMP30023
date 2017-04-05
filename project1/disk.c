  #include "functions.h"

  pronode_t * create_process(int time_cr, int pr_id, int mem_size, int job_time,
    pronode_t *ready){

    pronode_t *newpronode;
    process_t *newproc;
    newproc = malloc(sizeof(process_t));
    assert(newproc != NULL);
    newproc->time_cr = time_cr;
    newproc->pr_id = pr_id;
    newproc->mem_size = mem_size;
    newproc->job_time = job_time;
    newdisk->ready->process = newproc;

    if (disk==NULL){
      /*New List*/
      return new;
    }
    else{
      /*New Node for List*/
      pronode_t *curr;
      curr = head;
      while(curr->next != NULL){
        curr = curr->next;
      }
      curr->next = new;
      return head;
    }

}

disk_t * create_disk(){

  /**Initialise Disk**/
  disk_t *newdisk;
  pronode_t *ready;
  pronode_t *swap;

  newdisk = malloc(sizeof(disk_t));
  assert(newdisk!=NULL);
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
  new = malloc(sizeof(pronode_t));
  assert(new != NULL);
  new->process = NULL;
  new->next=NULL;
  return new;
}

void print_disk(pronode_t *head){
  pronode_t *curr;
  curr = head;
  while(curr != NULL){
    printf("%d, %d, %d, %d\n",curr->process->time_cr, curr->process->pr_id,
      curr->process->mem_size, curr->process->job_time);
      curr = curr->next;
  }
}

 pop_process(pronode_t *head, pronode_t *swap, int time, int num_p, int num_s){
  process_t *highest;
  process_t *temp;
  if(!num_s){
    if(!num_p){
      return
    }
    else{
      return NULL;
    }
  }


}

pronode_t *pop_out_process(pronode_t *list){


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

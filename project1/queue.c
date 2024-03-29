#include "functions.h"

queue_t *init_queue(){
  queue_t *new;
  new = malloc(sizeof(*new));
  assert(new!=NULL);
  new->head = NULL;
  new->foot = NULL;
  new->numitems = 0;
  return new;
}

queue_t *insert_at_foot(queue_t *queue, process_t *item){
  pronode_t *new = new_pronode(item);
  if(queue->foot != NULL){
    pronode_t *temp;
    temp = queue->foot;
    queue->foot = new;
    temp->next = new;
  }
  else{
    /*Queue Footer is Empty*/
    queue->head = new;
    queue->foot = new;
  }
  queue->numitems++;
  return queue;
}

process_t *pop_from_queue(queue_t **queue){
  assert((*queue)->head != NULL);

  pronode_t *pronod = (*queue)->head;
  process_t *tempproc = pronod->process;
  (*queue)->head = (*queue)->head->next;
  free_pronode(pronod);
  (*queue)->numitems--;

  if((*queue)->numitems == 1){
    (*queue)->head->next=NULL;
    (*queue)->foot = (*queue)->head;
  }
  else if(((*queue)->numitems == 0)){
    (*queue)->head = NULL;
    (*queue)->foot = NULL;
  }
  return tempproc;
}

process_t *pop_from_queue_select(queue_t **queue, process_t *pro){
  assert((*queue)->head != NULL);
  pronode_t *curr = (*queue)->head;
  pronode_t *prev = NULL;
  process_t *temp;

  if(curr->next == NULL){
    /*Item has to be the process in question*/
    temp = curr->process;
    (*queue)->head = NULL;
    (*queue)->foot = NULL;
    (*queue)->numitems--;
    free_pronode(curr);
    return temp;
  }

  while(curr->process->pr_id != pro->pr_id){
    prev = curr;
    curr = curr->next;
  }
  if (prev == NULL){
    /*Item is head of list*/
    (*queue)->head = curr->next;
    (*queue)->numitems--;
    temp = curr->process;
    free_pronode(curr);
    return temp;
  }

  temp = curr->process;
  prev->next = curr->next;
  (*queue)->numitems--;
  if(prev->next == NULL){
    (*queue)->foot = prev;
  }
  if((*queue)->numitems == 1){
    (*queue)->head = prev;
  }
  free_pronode(curr);
  return temp;
}


void printqueue(queue_t *q, int timer){
  pronode_t *qitem = q->head;
  while(qitem != NULL){
    printf("Process: %d in q - NEEDS:%d RUN: %d SIZE:%d TIME:%d TIMEOD: %d \n",qitem->process->pr_id,
  qitem->process->job_time, qitem->process->run_time,qitem->process->mem_size, timer, timer-qitem->process->time_memoryin);
    qitem = qitem->next;
  }
  fflush(stdout);
}

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
  process_t *temp = NULL;
  if((*queue)->head->process->pr_id == pro->pr_id){
    pronode_t *processa = (*queue)->head;
    (*queue)->head = (*queue)->head->next;
    (*queue)->numitems--;
    if((*queue)->numitems == 1){
      (*queue)->head->next=NULL;
      (*queue)->foot = (*queue)->head;
    }
    else if((*queue)->numitems == 0){
      (*queue)->head = NULL;
      (*queue)->foot = NULL;
    }
    temp = processa->process;
    free_pronode(processa);
    return temp;
  }

  pronode_t *curr = (*queue)->head->next;
  pronode_t *prev = (*queue)->head;

  while(curr!=NULL){

    if(curr->process->pr_id == pro->pr_id){
      prev->next = curr->next;
      temp = curr->process;
      (*queue)->numitems--;
      if((*queue)->numitems == 1){
        (*queue)->head->next=NULL;
        (*queue)->foot = (*queue)->head;
      }
      else if((*queue)->numitems == 0){
        (*queue)->head = NULL;
        (*queue)->foot = NULL;
      }
      free_pronode(curr);
      return temp;
    }
    curr = curr->next;
  }
  /*Shouldn't reach here*/
  return temp;
}

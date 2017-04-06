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

queue_t *insert_at_head(queue_t *queue, pronode_t *item) {
	if(queue->head != NULL){
    pronode_t *temp;
    temp=queue->head;
    queue->head = item;
    item->next = temp;
  }
  else{
    /*Queue is empty*/
    queue->head = item;
  }
  return queue;
}

queue_t *insert_at_foot(queue_t *queue, pronode_t *item){
  if(queue->foot != NULL){
    pronode_t *temp;
    temp = queue->foot;
    queue->foot = item;
    temp->next = item;
  }
  else{
    /*Queue Footer is Empty*/
    queue->foot = item;
  }
  return queue;
}

pronode_t *pop_from_queue(queue_t **queue){
  assert((*queue)->head != NULL);
  pronode_t *proc = (*queue)->head;
  proc->next = NULL;
  (*queue)->head = (*queue)->head->next;
  numitems--;
  if(numitems == 1){
    (*queue)->tail = (*queue)->head->next;
  }
  return proc;
}

pronode_t *pop_from_queue_select(queue_t **queue, pronode_t *pro){
  assert(queue->head != NULL);
  if(curr->process->pr_id == pro->process->pr_id){
    pronode_t *processa = (*queue)->head;
    processa->next = NULL;
    (*queue)->head = NULL;
    (*queue)->tail = NULL;
    (*queue)->numItems--;
    return processa;
  }

  pronode_t *curr = (*queue)->head->next;
  pronode_t *prev = (*queue)->head;

  while(curr!=NULL){
    if(curr->process->pr_id == pro->process->pr_id){
      prev->process->next = curr->process->next;
      curr->next = NULL;
      (*queue)->numitems--
      return curr;
    }
  }
  return curr;
}

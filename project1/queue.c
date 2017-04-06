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

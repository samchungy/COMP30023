/**CODE ADAPTED FROM ALISTAIR MOFFAT listops.c**/
#include "functions.h"

queue_t *init_queue(){
  queue_t *new;
  new = malloc(sizeof(*new));
  assert(new!=NULL);
  new->head = NULL;
  new->foot = NULL;
  return new;
}

queue_t *insert_at_head(queue_t *list, pronode_t *item) {
	if(list->head != NULL){
    pronode_t *curr;
    curr=list->head;
    list->head = item;
    item->next = curr;
  }
  else{
    /*Queue is empty*/
    list->head = item;
  }
  return list;
}

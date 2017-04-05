/**CODE ADAPTED FROM ALISTAIR MOFFAT listops.c**/
queue_t *init_queue(){
  pronode_t *new;
  new = malloc(sizeof(*new));
  assert(new!=NULL);
  new->head = NULL;
  new->foot = NULL;
  return new;
}


void free_list(queue_t *list) {
	pronode_t *curr, *prev;
	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	free(list);
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

queue_t *insert_at_foot(queue_t *list, data_t value) {
	pronode_t *new;
	new = (pronode_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	new->data = value;
	new->next = NULL;
	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = new;
	} else {
		list->foot->next = new;
		list->foot = new;
	}
	return list;
}

data_t
get_head(queue_t *list) {
	assert(list!=NULL && list->head!=NULL);
	return list->head->data;
}

queue_t
*get_tail(queue_t *list) {
	pronode_t *oldhead;
	assert(list!=NULL && list->head!=NULL);
	oldhead = list->head;
	list->head = list->head->next;
	if (list->head==NULL) {
		/* the only list node just got deleted */
		list->foot = NULL;
	}
	free(oldhead);
	return list;

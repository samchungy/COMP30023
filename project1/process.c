  #include "process.h"

node_t * read_to_disk(int time_cr, int pr_id, int mem_size, int job_time,
  node_t *head){

    node_t *new;
    new = malloc(sizeof(node_t));
    assert(new != NULL);
    new->process.time_cr = time_cr;
    new->process.pr_id = pr_id;
    new->process.mem_size = mem_size;
    new->process.job_time = job_time;
    new->next = NULL;
    node_t *curr;

    if (head==NULL){
      /*New List*/
      return new;
    }
    else{
      /*New Node for List*/
      curr = head;
      while(curr->next != NULL){
        curr = curr->next;
      }
      curr->next = new;
      return head;
    }

}

void print_disk(node_t *head){
  node_t *curr;
  curr = head;
  while(curr != NULL){
    printf("%d, %d, %d, %d\n",curr->process.time_cr, curr->process.pr_id,
      curr->process.mem_size, curr->process.job_time);
      curr = curr->next;
  }
}

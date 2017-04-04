  #include "process.h"

  pronode_t * read_to_disk(int time_cr, int pr_id, int mem_size, int job_time,
    pronode_t *head){
    pronode_t *curr = NULL;
    pronode_t *new = NULL;
    new = malloc(sizeof(pronode_t));
    assert(new != NULL);
    new->process.time_cr = time_cr;
    new->process.pr_id = pr_id;
    new->process.mem_size = mem_size;
    new->process.job_time = job_time;
    new->process.indisk = true;
    new->next = NULL;
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

void print_disk(pronode_t *head){
  pronode_t *curr;
  curr = head;
  while(curr != NULL){
    printf("%d, %d, %d, %d\n",curr->process.time_cr, curr->process.pr_id,
      curr->process.mem_size, curr->process.job_time);
      curr = curr->next;
  }
}

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

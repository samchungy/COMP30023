#include "process.h"

int main(int argc, char **argv){
  int time_cr;
  int pr_id;
  int mem_size;
  int job_time;
  int numprocesses = 0;
  node_t *head;

  while(scanf("%d",&time_cr)!=EOF){
    scanf("%d", &pr_id);
    scanf("%d", &mem_size);
    scanf("%d", &job_time);
    numprocesses++;
    head = read_to_disk(time_cr, pr_id, mem_size, job_time, head);
    print_disk(head);
    /*printf("%d, %d, %d, %d\n",time_cr, pr_id, mem_size, job_time);*/
  }
  return 0;
}

void start_simulation(){

}

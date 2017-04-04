#include "process.h"

int main(int argc, char *argv[]){
  int time_cr;
  int pr_id;
  int mem_size;
  int job_time;
  int numprocesses = 0;
  char filename[20];
  node_t *head;

  while ((option = getopt(argc, argv,"f:a:m:q:")) != -1) {
          switch (option) {
               case 'f' : area = ;
                   break;
               case 'p' : perimeter = 0;
                   break;
               case 'l' : length = atoi(optarg);
                   break;
               case 'b' : breadth = atoi(optarg);
                   break;
               default: print_usage();
                   exit(EXIT_FAILURE);
          }

  while(scanf("%d",&time_cr)!=EOF){
    scanf("%d", &pr_id);
    scanf("%d", &mem_size);
    scanf("%d", &job_time);
    numprocesses++;
    head = read_to_disk(time_cr, pr_id, mem_size, job_time, head);
    /*printf("%d, %d, %d, %d\n",time_cr, pr_id, mem_size, job_time);*/
  }
  print_disk(head);
  return 0;
}

void start_simulation(){

}

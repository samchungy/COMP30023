#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node_t{
  int size;
  int start;
  int end;
  struct node_t *next;
}node_t;

int main(int argc, char *argv[]){
  node_t *new;
  new = malloc(sizeof(node_t));
  assert(new!=NULL);
  new->size = 10;
  new->start = 1;
  new->end = 1;
  new->next = NULL;
  changedata(&new);
  printf("%d",new->size);
  return 0;
}

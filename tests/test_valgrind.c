#include <stdio.h>
#include <stdlib.h>

#include "mem.h"

void* alloc(size_t size){
  void *a = mem_alloc(size);
  printf("Allocated %ld bytes at : %p\n",size,a);
  return a;
}

void free_alloc(void *a){
  mem_free(a);
  printf("Freed %p\n",a);
}

int main(int argc,char **argv){
  mem_init();
  void *a = alloc(455), *b = alloc(333);
  free_alloc(a);
  a = NULL;
  b = NULL;
  b++;
  return 0;
}

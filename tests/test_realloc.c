#include <stdio.h>
#include <stdlib.h>

#include "mem.h"

void* alloc(size_t size){
  void *a = mem_alloc(size);
  printf("Allocated %ld bytes at : %p\n",size,a);
  return a;
}

void* myRealloc(void *ptr,size_t size){
  void *a = mem_realloc(ptr,size);
  printf("Reallocated %ld bytes at : %p\n",size,a);
  return a;
}

void free_alloc(void *a){
  mem_free(a);
  printf("Freed %p\n",a);
}

void afficher_zone(void *adresse, size_t taille, int free)
{
    printf("Zone %s, Adresse : %lu, Taille : %lu\n", free?"libre":"occupee",
           (unsigned long) adresse, (unsigned long) taille);
}

int main(int argc,char **argv){
  mem_init();
  mem_show(afficher_zone);
  void *a = alloc(455), *b = alloc(350),*c = alloc(1240);
  mem_show(afficher_zone);
  a = myRealloc(a,322);
  free_alloc(b);
  mem_show(afficher_zone);
  a = myRealloc(a,1024);
  mem_show(afficher_zone);
  free_alloc(a);
  free_alloc(c);
  mem_show(afficher_zone);
  return 0;
}

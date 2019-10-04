#include <stdio.h>
#include <stdlib.h>
#include <valgrind/valgrind.h>

int main(int argc , char ** argv){
  char pool[100];
  VALGRIND_CREATE_MEMPOOL(pool, 0, 0);
  VALGRIND_MEMPOOL_ALLOC(pool, pool, 8);
  VALGRIND_MEMPOOL_FREE(pool, pool);
  VALGRIND_DESTROY_MEMPOOL(pool);
  return 0;
}

#include "mem.h"
//-------------------------------------------------------------
// mem_init
//-------------------------------------------------------------
void mem_init() {
  mem_fit(&mem_first_fit);

  fb **ffb = (fb**)((mem_fit_function_t **) get_memory_adr() + sizeof(mem_fit_function_t*));
  *ffb = (fb*)(ffb + sizeof(fb*));
  fb *fb = *ffb;
  fb->size = MEMORY_SIZE - sizeof(mem_fit_function_t *);
  fb->next = NULL;
}

//-------------------------------------------------------------
// mem_alloc
//-------------------------------------------------------------
void* mem_alloc(size_t size) {
  /* A COMPLETER */
  return NULL;
}

//-------------------------------------------------------------
// mem_free
//-------------------------------------------------------------
void mem_free(void* zone) {
   /* A COMPLETER */
}

//-------------------------------------------------------------
// Itérateur(parcours) sur le contenu de l'allocateur
// mem_show
//-------------------------------------------------------------
void mem_show(void (*print)(void *, size_t, int free)) {
   /* A COMPLETER */
}

//-------------------------------------------------------------
// mem_fit
//-------------------------------------------------------------
void mem_fit(mem_fit_function_t* mff) {
  mem_fit_function_t** fit_function = (mem_fit_function_t **) get_memory_adr();
  *fit_function = mff;
}

//-------------------------------------------------------------
// Stratégies d'allocation
//-------------------------------------------------------------
struct fb* mem_first_fit(struct fb* head, size_t size) {
  fb *b = head;
  while(b->size < size && b->next != NULL) b = b->next;
  return (b->size >= size)?b:NULL;
}
//-------------------------------------------------------------
struct fb* mem_best_fit(struct fb* head, size_t size) {
  fb *best = NULL, *b = head;
  while(b->next != NULL){
    if(b->size >= size)
      best = (best == NULL)?b:((b->size < best->size)?b:best);
    b = b->next;
  }
  return best;
}
//-------------------------------------------------------------
struct fb* mem_worst_fit(struct fb* head, size_t size) {
  fb *best = NULL, *b = head;
  while(b->next != NULL){
    if(b->size >= size)
      best = (best == NULL)?b:((b->size > best->size)?b:best);
    b = b->next;
  }
  return best;
}

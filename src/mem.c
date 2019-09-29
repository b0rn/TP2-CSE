#include "mem.h"
//-------------------------------------------------------------
// mem_init
//-------------------------------------------------------------
void mem_init() {
  mem_fit(&mem_first_fit);// on initialise la fonction fit

  fb **ffb = (fb**)((mem_fit_function_t **) get_memory_adr() + sizeof(mem_fit_function_t*));
  *ffb = (fb*)(ffb + sizeof(fb*));// on place le pointeur vers le pointeur de la premiere zone vide
  fb *fictivefb = *ffb;// on met notre première zone vide fictive
  fictivefb->size = sizeof(fb);
  fictivefb->next = fictivefb + sizeof(fb);
  fictivefb->next->size = MEMORY_SIZE - sizeof(mem_fit_function_t*) - sizeof(fb*) - sizeof(fb);
  fictivefb->next->next = NULL;
}

//-------------------------------------------------------------
// mem_alloc
//-------------------------------------------------------------
void* mem_alloc(size_t size) {
  mem_fit_function_t **mff = (mem_fit_function_t **) get_memory_adr();// on récupère la fonction de fit

  size += sizeof(size_t);// on met la taille du bloc au début
  size += (size % ALIGNMENT);// on aligne le bloc

  if(size < sizeof(fb))
    size += sizeof(fb) - size;

  fb **head = (fb**)(mff + sizeof(mem_fit_function_t*));// on récupère la tête
  fb *previousB = (**mff)(*head,size);// on récupère le bloc précédent le bloc vide qui nous interesse

  if(previousB != NULL && previousB->next != NULL){
    fb *b = previousB->next, *next = b->next;// on récupère le bloc vide qui convient à notre strat

    // s'il ne reste pas assez de place pour stocker les infos du bloc libre dans la zone libre restante
    if(b->size - size < sizeof(fb)){
        size = b->size;
    }
    else if((void*) b + size < (get_memory_adr() + get_memory_size())){
      // sinon , on crée un nouveau bloc libre
      fb *bNext = b->next;
      next = (void *) b +size;
      next->size = b->size - size;
      next->next = bNext;
    }
    previousB->next = next;// on lie le bloc précédent au bloc suivant notre bloc à allouer

    *((size_t *)b) = size;// on met la taille allouée au début du bloc
    return (void *)b+sizeof(size_t);// on retourne l'adresse qui est après la taille
  }
  return NULL;
}

//-------------------------------------------------------------
// mem_free
//-------------------------------------------------------------
void mem_free(void* zone) {
  size_t size = *(size_t*)(zone - sizeof(size_t));// on récupère la taille de la zone

  fb *newFb = (fb*)(zone - sizeof(size_t));// On récupère l'adresse ou placer notre nouveau bloc vide
  newFb->size = size;
  newFb->next = NULL;

  fb **head = (fb**)(((mem_fit_function_t **) get_memory_adr()) + sizeof(mem_fit_function_t*));// on récupère la tête
  fb *b = (*head)->next;// on saute la première zone libre (fictive)
  fb *tmpB = newFb;

  if(b == NULL){ // si on a pas de bloc libre
    (*head)->next = newFb;// on lie la tête au nouveau bloc
    return;
  }

  if(b > newFb){ // si notre premier bloc libre est après celui qu'on vient de libérer
    (*head)->next = newFb;
    newFb->next = b;
    tmpB = b;
    b = newFb;
  }else if(b->next == NULL){ // si on a qu'une seule zone libre
    if((void*)b+b->size == newFb) // si zone libre est collée à la nouvelle zone libre
      b->size += newFb->size;
    else
      b->next = newFb;
    return;
  }

  while(tmpB != NULL && b != NULL){
    if(b->next >= tmpB){ // si le bloc suivant est après la zone à traiter
      if(tmpB == newFb){ // si on traite la zone à libérer
        // on insère la nouvelle zone libre dans la liste chainée
        tmpB->next = b->next;
        b->next = tmpB;
      }

      if((void*)b + b->size == tmpB){ // si la zone à traiter et collée à la zone actuelle
        b->size += tmpB->size;
        b->next = tmpB->next;
        tmpB = b->next;
      }else if(tmpB == newFb){
        b = tmpB;
        tmpB = b->next;
      }else
        tmpB = NULL;// arret de la boucle
    }else
      b = b->next;// on continue à chercher le bloc qui précède notre nouvelle zone
  }
}

//-------------------------------------------------------------
// mem_get_size
//-------------------------------------------------------------
size_t mem_get_size(void *zone){
  return *(((size_t *)zone) - sizeof(size_t));
}

//-------------------------------------------------------------
// Itérateur(parcours) sur le contenu de l'allocateur
// mem_show
//-------------------------------------------------------------
void mem_show(void (*print)(void *, size_t, int free)) {
  fb **head = (fb**)(((mem_fit_function_t **) get_memory_adr()) + sizeof(mem_fit_function_t*));// on récupère la tête
  fb *lastFb = *head;
  void *adr = ((fb*)(head + sizeof(fb*)) + sizeof(fb));
  void *endadr = get_memory_adr() + get_memory_size();

  while(adr < endadr){
    if(lastFb->next == (fb*)adr){ // si on est sur une zone libre
      print((void *)(adr - get_memory_adr()),lastFb->next->size,1);
      adr = (void *)(lastFb->next+lastFb->next->size);
      lastFb = lastFb->next;
    }else{ // si on est sur une zone occupée
      size_t size = *((size_t*)adr);
      print((void*)((adr + sizeof(size_t)) - get_memory_adr()),size,0);
      adr = adr + size;
    }
  }
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
  while(b->next != NULL && b->next->size < size) b = b->next;
  return (b->next != NULL)?b:NULL;
}
//-------------------------------------------------------------
struct fb* mem_best_fit(struct fb* head, size_t size) {
  fb *best = NULL, *b = head;
  while(b->next != NULL){
    if(b->next->size >= size)
      best = (best == NULL)?b->next:((b->next->size < best->size)?b->next:best);
    b = b->next;
  }
  return best;
}
//-------------------------------------------------------------
struct fb* mem_worst_fit(struct fb* head, size_t size) {
  fb *best = NULL, *b = head;
  while(b->next != NULL){
    if(b->next->size >= size)
      best = (best == NULL)?b->next:((b->next->size > best->size)?b->next:best);
    b = b->next;
  }
  return best;
}

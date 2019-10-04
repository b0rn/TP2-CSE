#if !defined(__MEM_H)
#define __MEM_H
#include <stdio.h>
#include <stddef.h>
#include "mem_os.h"
#include "common.h"
#include "fb.h"

/* -----------------------------------------------*/
/* Interface utilisateur de votre allocateur      */
/* -----------------------------------------------*/
void mem_init(void);
void* mem_alloc(size_t);
void mem_free(void*);
size_t mem_get_size(void *);

/* Itérateur sur le contenu de l'allocateur */
void mem_show(void (*print)(void *, size_t, int free));
void mem_fit(mem_fit_function_t* mff);
struct fb* mem_first_fit(struct fb* head, size_t size);
struct fb* mem_best_fit(struct fb* head, size_t size);
struct fb* mem_worst_fit(struct fb* head, size_t size);
#endif

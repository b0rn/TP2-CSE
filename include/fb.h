#ifndef FB_H_INCLUDED
#define FB_H_INCLUDED

#include <stddef.h>

typedef struct fb{
    size_t size;
    fb *next;
} fb;

#endif // FB_H_INCLUDED

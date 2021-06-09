#ifndef _MALLOC_H_
#define _MALLOC_H_

#include <stdlib.h>

#define shift_address(addr, size) ((void *)((char *)(addr) + (size)))
#define is_allocated(x) (*((char *)(x)) & 1)
#define sizeof_chunk(x) (*((size_t *)(x)) ^ 1)
#define allocate(x) (*((char *)(x)) += 1)
#define unallocate(x) (*((char *)(x)) -= 1)

size_t align(size_t size);
void *naive_malloc(size_t size);
void *_malloc(size_t size);
void _free(void *ptr);
size_t largest;
#endif /* _MALLOC_H_ */

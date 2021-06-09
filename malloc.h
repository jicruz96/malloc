#ifndef _MALLOC_H_
#define _MALLOC_H_

#include <stdlib.h>

#define shift_address(addr, size) ((void *)((char *)(addr) + (size)))
#define is_allocated(x) (*((size_t *)(x)) & 1)
#define sizeof_chunk(x) ((*((size_t *)(x)) >> 1) << 1)
#define next_chunk(chunk) shift_address(chunk, sizeof_chunk(chunk))
#define allocate(x) (*((size_t *)(x)) |= 1)
#define unallocate(x) (*((size_t *)(x)) ^= 1)

size_t align(size_t size);
void *naive_malloc(size_t size);
void *_malloc(size_t size);
void _free(void *ptr);
void *return_existing_chunk(void *tmp, size_t chunk_size);
size_t largest_available_chunk_size;
#endif /* _MALLOC_H_ */

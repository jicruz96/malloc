#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "malloc.h"



size_t largest = 0;

/**
 * align - returns the page-aligned value of a quantity
 * @size: size to align
 * Return: aligned size
 **/
size_t align(size_t size)
{
	if (!size)
		return (0);
	if (size < 8)
		return (8);
	if (size % 8 == 0)
		return (size);
	return (size + (8 - (size % 8)));
}

/**
 * _malloc - cusotm-built malloc
 * @size: number of bytes to allocate
 *
 * Return: the memory address newly allocated, or NUll on error
 *
 * Note: for learning purposes only
 **/
void *_malloc(size_t size)
{
	size_t chunk_size = align(size + sizeof(size_t));
	static void *first_spot, *next_spot;
	static size_t largest, available, allocated;
	void *tmp;
	static long pagesize;

	if (size == 0)
		return (NULL);
	if (chunk_size < largest)
	{
		for (tmp = first_spot; is_allocated(tmp); )
			tmp = shift_address(tmp, sizeof_chunk(tmp));
		return (tmp);
	}
	while (available < chunk_size)
	{
		if (!first_spot)
		{
			pagesize = sysconf(_SC_PAGESIZE);
			first_spot = sbrk(pagesize), next_spot = first_spot;
			if (next_spot == (void *)-1)
			{
				next_spot = NULL, first_spot = NULL;
				return (NULL);
			}
		}
		else if (sbrk(pagesize) == (void *)-1)
		{
			return (NULL);
		}
		allocated += pagesize;
		available += pagesize;
	}
	tmp = next_spot;
	memcpy(tmp, &chunk_size, sizeof(chunk_size));
	allocate(tmp);
	next_spot = shift_address(tmp, chunk_size);
	available -= chunk_size;
	return (shift_address(tmp, sizeof(chunk_size)));
}

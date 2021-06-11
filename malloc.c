#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "malloc.h"



size_t largest_available_chunk_size = 0;

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
	static size_t available;
	void *tmp;
	static long pagesize;

	if (size == 0)
		return (NULL);

	if (chunk_size <= largest_available_chunk_size)
		return (return_existing_chunk(first_spot, chunk_size));

	while (available < chunk_size)
	{
		if (!first_spot)
		{
			pagesize = sysconf(_SC_PAGESIZE);
			first_spot = sbrk(pagesize);
			if (first_spot == (void *)-1)
			{
				next_spot = NULL;
				first_spot = NULL;
				return (NULL);
			}
			next_spot = first_spot;
		}
		else if (sbrk(pagesize) == (void *)-1)
		{
			return (NULL);
		}

		available += pagesize;
	}

	tmp = next_spot;
	memcpy(tmp, &chunk_size, sizeof(chunk_size));
	allocate(tmp);
	next_spot = next_chunk(tmp);
	available -= chunk_size;
	return (shift_address(tmp, sizeof(chunk_size)));
}

/**
 * return_existing_chunk - checks init'd data segment for available chunks
 * @tmp: pointer to current chunk
 * @chunk_size: desired chunk size
 * Return: pointer to a chunk of size equal or greater to chunk_size
 **/
void *return_existing_chunk(void *tmp, size_t chunk_size)
{
	void *tmp2;
	size_t yes_indeed;
	static size_t tmp_largest;


	if (!is_allocated(tmp) && sizeof_chunk(tmp) >= chunk_size)
	{
		yes_indeed = (sizeof_chunk(tmp) == largest_available_chunk_size);
		tmp_largest = sizeof_chunk(tmp);
		memcpy(tmp, &chunk_size, sizeof(chunk_size));
		allocate(tmp);
		if (tmp_largest > chunk_size && (tmp_largest - chunk_size) > 8)
		{
			tmp2 = next_chunk(tmp);
			tmp_largest = tmp_largest - chunk_size;
			memcpy(tmp2, &tmp_largest, sizeof(tmp_largest));
		}

		if (yes_indeed)
		{
			tmp_largest = sizeof_chunk(tmp2);
			for (tmp2 = next_chunk(tmp); sizeof_chunk(tmp2); tmp2 = next_chunk(tmp2))
				if (!is_allocated(tmp2) && sizeof_chunk(tmp2) > tmp_largest)
					tmp_largest = sizeof_chunk(tmp2);
			largest_available_chunk_size = tmp_largest;
		}
		tmp_largest  = 0;
		return (shift_address(tmp, sizeof(size_t)));
	}

	if (!is_allocated(tmp) && sizeof_chunk(tmp) > tmp_largest)
		tmp_largest = sizeof_chunk(tmp);

	return (return_existing_chunk(next_chunk(tmp), chunk_size));
}

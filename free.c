#include "malloc.h"
#include <stdio.h>
/**
 * _free - free replica
 * @ptr: pointer to free
 **/
void _free(void *ptr)
{
	void *tmp;

	if (ptr)
	{
		tmp = shift_address(ptr,  -1 * sizeof(size_t));
		unallocate(tmp);
		if (sizeof_chunk(tmp) > largest_available_chunk_size)
			largest_available_chunk_size = sizeof_chunk(tmp);
	}
}

#include "malloc.h"

/**
 * _free - free replica
 * @ptr: pointer to free
 **/
void _free(void *ptr)
{
	if (ptr)
	{
		unallocate(ptr);
		if (sizeof_chunk(ptr) > largest)
			largest = sizeof_chunk(ptr);
	}
}

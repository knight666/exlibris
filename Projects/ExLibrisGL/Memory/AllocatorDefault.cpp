#include "AllocatorDefault.h"

namespace ExLibris {

	void* AllocatorDefault::Allocate(const char* tag, size_t size)
	{
		return malloc(size);
	}

	void* AllocatorDefault::Reallocate(const char* tag, void* allocated, size_t size)
	{
		return realloc(allocated, size);
	}

	void AllocatorDefault::Free(const char* tag, void* allocated)
	{
		free(allocated);
	}

};
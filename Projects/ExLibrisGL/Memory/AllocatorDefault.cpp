#include "AllocatorDefault.h"

namespace ExLibris {

	void* AllocatorDefault::Allocate(size_t size)
	{
		return malloc(size);
	}

	void AllocatorDefault::Free(void* allocated)
	{
		free(allocated);
	}

};

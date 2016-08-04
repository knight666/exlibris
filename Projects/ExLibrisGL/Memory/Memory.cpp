#include "Memory.h"

namespace ExLibris {

	static IAllocator* g_Allocator;

	IAllocator* GetAllocator()
	{
		return g_Allocator;
	}

	void InstallAllocator(IAllocator* allocator)
	{
		g_Allocator = allocator;
	}

}; // namespace ExLibris
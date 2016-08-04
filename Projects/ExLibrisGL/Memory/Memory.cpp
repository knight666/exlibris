#include "Base.h"

#include "Memory/Memory.h"

namespace ExLibris {

	static IAllocator* g_Allocator = nullptr;

	IAllocator* GetAllocator()
	{
		return g_Allocator;
	}

	void InstallAllocator(IAllocator* allocator)
	{
		g_Allocator = allocator;
	}

}; // namespace ExLibris
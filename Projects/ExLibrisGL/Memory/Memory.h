#pragma once

#include <memory>

#include "IAllocator.h"

namespace ExLibris {

	IAllocator* GetAllocator();
	void InstallAllocator(IAllocator* allocator);

};

__forceinline void* operator new (size_t size){ return ExLibris::GetAllocator()->Allocate(size); }
__forceinline void* operator new [](size_t size) { return ExLibris::GetAllocator()->Allocate(size); }
__forceinline void* operator new(std::size_t size, const std::nothrow_t&){ return ExLibris::GetAllocator()->Allocate(size); }
__forceinline void* operator new [](std::size_t size, const std::nothrow_t&) { return ExLibris::GetAllocator()->Allocate(size); }
__forceinline void operator delete (void* p) _THROW0() { return ExLibris::GetAllocator()->Free(p); }
__forceinline void operator delete[](void* pArray) _THROW0() { return ExLibris::GetAllocator()->Free(pArray); }
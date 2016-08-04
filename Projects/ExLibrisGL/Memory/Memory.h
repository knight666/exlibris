#pragma once

#include "IAllocator.h"

namespace ExLibris {

	IAllocator* GetAllocator();
	void InstallAllocator(IAllocator* allocator);

};

#if EXL_OVERLOAD_NEW
	__forceinline void* operator new (size_t size){ return ExLibris::GetAllocator()->Allocate("exlibris", size); }
	__forceinline void* operator new [](size_t size) { return ExLibris::GetAllocator()->Allocate("exlibris", size); }
	__forceinline void* operator new(std::size_t size, const std::nothrow_t&){ return ExLibris::GetAllocator()->Allocate("exlibris", size); }
	__forceinline void* operator new [](std::size_t size, const std::nothrow_t&) { return ExLibris::GetAllocator()->Allocate("exlibris", size); }
	__forceinline void operator delete (void* data) _THROW0() { return ExLibris::GetAllocator()->Free("exlibris", data); }
	__forceinline void operator delete[](void* arrayData) _THROW0() { return ExLibris::GetAllocator()->Free("exlibris", arrayData); }
#else
	void* operator new (size_t size);
	void* operator new [](size_t size);
	void* operator new(std::size_t size, const std::nothrow_t&);
	void* operator new [](std::size_t size, const std::nothrow_t&);
	void operator delete (void* data) _THROW0();
	void operator delete[](void* arrayData) _THROW0();
#endif
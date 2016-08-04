#pragma once

#include "Base.h"

namespace ExLibris {

	class AllocatorDefault
		: public IAllocator
	{

	public:
		virtual void* Allocate(const char* tag, size_t size) override;
		virtual void* Reallocate(const char* tag, void* allocated, size_t size) override;
		virtual void Free(const char* tag, void* allocated) override;

	};

};
#pragma once

#include "Base.h"

namespace ExLibris {

	class AllocatorDefault
		: public IAllocator
	{

	public:
		virtual void* Allocate(size_t size) override;
		virtual void* Reallocate(void* allocated, size_t size) override;
		virtual void Free(void* allocated) override;

	};

};
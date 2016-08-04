#pragma once

namespace ExLibris {

	class IAllocator
	{

	public:
		virtual ~IAllocator() { }

		virtual void* Allocate(size_t size) = 0;
		virtual void Free(void* allocated) = 0;

	};

};
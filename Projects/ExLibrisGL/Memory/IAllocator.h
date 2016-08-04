#pragma once

namespace ExLibris {

	class IAllocator
	{

	public:
		virtual ~IAllocator() { }

		virtual void* Allocate(const char* tag, size_t size) = 0;
		virtual void* Reallocate(const char* tag, void* allocated, size_t size) = 0;
		virtual void Free(const char* tag, void* allocated) = 0;

	};

};
// STL

#include <iostream>
#include <map>
#include <string>
#include <vector>

// Windows

#include <windows.h>

// Framework

#include <Application.h>
#include <DebugHelper.h>

namespace fw = Framework;

// ExLibris

#include <Memory/AllocatorDefault.h>
#include <Library.h>

namespace exl = ExLibris;

class TraceAllocator
	: public exl::IAllocator
{

public:
	struct Allocation
	{
		char tag[32];
		size_t location;
		size_t size;
	};

	struct TagBucket
	{
		char tag[32];
		size_t allocated;
	};

public:
	TraceAllocator()
		: m_AllocationCursor(0)
		, m_BucketCursor(0)
		, m_TotalAllocated(0)
	{
		memset(m_Allocations, 0, sizeof(m_Allocations));
	}

	virtual void* Allocate(const char* tag, size_t size) override
	{
		if (m_AllocationCursor >= MaxAllocations)
		{
			return nullptr;
		}

		void* result = malloc(size);

		Allocation& current = m_Allocations[m_AllocationCursor++];
		strcpy_s(current.tag, tag);
		current.location = reinterpret_cast<size_t>(result);
		current.size = size;

		TagBucket* bucket = CreateBucket(tag);
		bucket->allocated += size;

		m_TotalAllocated += size;

		return result;
	}

	virtual void* Reallocate(const char* tag, void* allocated, size_t size) override
	{
		void* result = realloc(allocated, size);

		TagBucket* bucket = CreateBucket(tag);
		size_t location = reinterpret_cast<size_t>(allocated);

		Allocation* src = m_Allocations;
		for (size_t i = 0; i < m_AllocationCursor; ++i)
		{
			if (src->location == location)
			{
				bucket->allocated -= src->size;
				m_TotalAllocated -= src->size;

				break;
			}

			src++;
		}

		src->size = size;
		src->location = location;

		bucket->allocated += size;
		m_TotalAllocated += size;

		return result;
	}

	virtual void Free(const char* tag, void* allocated) override
	{
		free(allocated);

		TagBucket* bucket = CreateBucket(tag);
		size_t location = reinterpret_cast<size_t>(allocated);

		Allocation* src = m_Allocations;
		for (size_t i = 0; i < m_AllocationCursor; ++i)
		{
			if (src->location == location)
			{
				bucket->allocated -= src->size;
				m_TotalAllocated -= src->size;

				if (m_AllocationCursor > 1)
				{
					memcpy(src, &m_Allocations[m_AllocationCursor - 1], sizeof(Allocation));
				}

				m_AllocationCursor--;

				break;
			}

			src++;
		}
	}

	TagBucket* CreateBucket(const char* name)
	{
		TagBucket* bucket = GetBucketByName(name);

		if (bucket == nullptr &&
			m_BucketCursor < MaxBuckets)
		{
			bucket = &m_Buckets[m_BucketCursor++];

			strcpy_s(bucket->tag, name);
			bucket->allocated = 0;
		}

		return bucket;
	}

	TagBucket* GetBucketByName(const char* name)
	{
		TagBucket* src = m_Buckets;
		size_t name_length = strlen(name);

		for (size_t i = 0; i < m_BucketCursor; ++i)
		{
			if (!strncmp(src->tag, name, name_length))
			{
				return src;
			}

			src++;
		}

		return nullptr;
	}

	size_t GetBucketCount() const
	{
		return m_BucketCursor;
	}

	TagBucket* GetBucketByIndex(size_t index)
	{
		return (index >= m_BucketCursor) ? nullptr : &m_Buckets[index];
	}

private:
	static const size_t MaxAllocations = 4096;
	static const size_t MaxBuckets = 64;

	Allocation m_Allocations[MaxAllocations];
	size_t m_AllocationCursor;

	TagBucket m_Buckets[MaxBuckets];
	size_t m_BucketCursor;

	size_t m_TotalAllocated;

};

class Example
	: public fw::Application
{

public:

	Example(TraceAllocator& allocator, int argumentCount, const char** arguments)
		: fw::Application(argumentCount, arguments)
		, m_Allocator(allocator)
	{
	}

	~Example()
	{
	}

	bool ParseCommandLine(int argumentCount, const char** arguments)
	{
		return true;
	}

	bool Initialize()
	{
		m_Library = new exl::Library();

		try
		{
			m_DebugHelper = new fw::DebugHelper(m_Library);
		}
		catch (std::exception& e)
		{
			MessageBoxA(0, e.what(), "Error while creating TextHelper", MB_OK);
			return false;
		}

		return true;
	}

	void Update(float a_DeltaTime)
	{
	}

	void Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int width, height;
		glfwGetFramebufferSize(GetWindow(), &width, &height);

		m_DebugHelper->Clear();

		glm::vec2 cursor(20.0f, 20.0f);
		float line_offset = 20.0f;

		for (size_t i = 0; i < m_Allocator.GetBucketCount(); ++i)
		{
			TraceAllocator::TagBucket* bucket = m_Allocator.GetBucketByIndex(i);

			char buffer[128] = { 0 };
			sprintf_s(buffer, "%s: %d bytes", bucket->tag, bucket->allocated);
			m_DebugHelper->AddText(buffer, cursor);

			cursor.y += line_offset;
		}

		m_DebugHelper->Render(width, height);
	}

	void Destroy()
	{
		delete m_DebugHelper;
		delete m_Library;
	}

private:

	TraceAllocator& m_Allocator;
	exl::Library* m_Library;
	fw::DebugHelper* m_DebugHelper;

}; // class Example

int main(int argc, const char** argv)
{
	TraceAllocator allocator;
	exl::InstallAllocator(&allocator);

	Example application(allocator, argc, argv);
	return application.Run();
}
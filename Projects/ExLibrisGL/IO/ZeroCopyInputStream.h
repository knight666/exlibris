#pragma once

namespace ExLibris
{

	class ZeroCopyInputStream
	{

	public:

		virtual ~ZeroCopyInputStream()
		{
		}

		virtual int GetBytesRead() const = 0;

		virtual bool Next(const char** a_Data, int* a_Size) = 0;
		virtual bool Rewind(int a_Count) = 0;
		virtual bool Skip(int a_Count) = 0;

	}; // class ZeroCopyInputStream

}; // namespace ExLibris
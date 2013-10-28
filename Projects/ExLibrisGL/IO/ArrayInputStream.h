#pragma once

#include "ZeroCopyInputStream.h"

namespace ExLibris
{

	class ArrayInputStream
		: public ZeroCopyInputStream
	{

	public:

		ArrayInputStream(const char* a_Data, int a_Size, int a_BlockSize = -1);
		~ArrayInputStream();

		int GetBytesRead() const;

		bool Next(const char** a_Data, int* a_Size);
		bool Rewind(int a_Count);
		bool Skip(int a_Count);

	private:

		const char* m_Data;
		const int m_Size;
		int m_Cursor;
		const int m_BlockSize;

		int m_LastReturnedSize;

	}; // class ArrayInputStream

}; // namespace ExLibris
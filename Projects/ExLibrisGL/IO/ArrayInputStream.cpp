#include "ExLibrisGL.PCH.h"

#include "ArrayInputStream.h"

namespace ExLibris
{

	ArrayInputStream::ArrayInputStream(const char* a_Data, int a_Size, int a_BlockSize /*= -1*/)
		: m_Data(a_Data)
		, m_Cursor(0)
		, m_Size(a_Size)
		, m_BlockSize(a_BlockSize > 0 ? a_BlockSize : a_Size)
		, m_LastReturnedSize(0)
	{
	}

	ArrayInputStream::~ArrayInputStream()
	{
	}

	int ArrayInputStream::GetBytesRead() const
	{
		return m_Cursor;
	}

	bool ArrayInputStream::Next(const char** a_Data, int* a_Size)
	{
		if (m_Cursor >= m_Size)
		{
			m_LastReturnedSize = 0;
			return false;
		}

		m_LastReturnedSize = std::min(m_BlockSize, m_Size - m_Cursor);

		*a_Data = m_Data + m_Cursor;
		*a_Size = m_LastReturnedSize;
		m_Cursor += m_LastReturnedSize;

		return true;
	}

	bool ArrayInputStream::Rewind(int a_Count)
	{
		if (a_Count <= 0 || a_Count > m_LastReturnedSize || m_LastReturnedSize <= 0)
		{
			return false;
		}

		m_Cursor -= a_Count;
		m_LastReturnedSize = 0;

		return true;
	}

	bool ArrayInputStream::Skip(int a_Count)
	{
		if (a_Count <= 0)
		{
			return false;
		}

		m_LastReturnedSize = 0;

		if (a_Count > m_Size - m_Cursor)
		{
			m_Cursor = m_Size;

			return false;
		}

		m_Cursor += a_Count;

		return true;
	}

}; // namespace ExLibris
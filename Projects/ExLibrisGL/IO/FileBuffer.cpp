/*
 * This file is a part of the ExLibris project.
 *
 * Copyright (C) 2013 Quinten Lansu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 */

#include "ExLibrisGL.PCH.h"

#include "FileBuffer.h"

namespace ExLibris
{

	FileBuffer::FileBuffer(FILE* a_Handle, Ownership a_Ownership /*= eOwnership_Owned*/, std::size_t a_BufferSize /*= 256*/, std::size_t a_PutBackSize /*= 1*/)
		: m_Handle(a_Handle)
		, m_Ownership(a_Ownership)
		, m_PutBackSize(std::max<std::size_t>(a_PutBackSize, 1))
		, m_BufferSize(std::max<std::size_t>(a_BufferSize, m_PutBackSize) + m_PutBackSize)
		, m_Cursor(0)
	{
		m_Buffer = new char[m_BufferSize];
		char* end = &m_Buffer[m_BufferSize];

		setg(end, end, end);
	}

	FileBuffer::~FileBuffer()
	{
		delete [] m_Buffer;

		if (m_Handle != nullptr && m_Ownership == eOwnership_Owned)
		{
			std::fclose(m_Handle);
		}
	}

	FileBuffer::int_type FileBuffer::underflow()
	{
		// still reading from buffer
		if (gptr() < egptr())
		{
			return traits_type::to_int_type(*gptr());
		}

		char* base = m_Buffer;
		char* start = base;

		// check if not the first fill
		if (eback() == base)
		{
			std::memmove(base, egptr() - m_PutBackSize, m_PutBackSize);
			start += m_PutBackSize;
		}

		std::size_t read = std::fread(start, 1, m_BufferSize, m_Handle);
		if (read == 0)
		{
			return traits_type::eof();
		}

		setg(base, start, start + read);

		return traits_type::to_int_type(*gptr());
	}

	FileBuffer::pos_type FileBuffer::seekpos(FileBuffer::pos_type a_Position, std::ios_base::open_mode a_Mode)
	{
		if (std::fseek(m_Handle, (long)a_Position, SEEK_SET) == 0)
		{
			return std::streambuf::pos_type(std::_BADOFF);
		}

		return (std::streambuf::pos_type)std::ftell(m_Handle);
	}

	FileBuffer::pos_type FileBuffer::seekoff(FileBuffer::off_type a_Offset, std::ios_base::seekdir a_Direction, std::ios_base::openmode a_Mode)
	{
		char* current_gptr = gptr();
		char* current_egptr = egptr();

		switch (a_Direction)
		{

		case std::ios_base::beg:
			{
				std::fseek(m_Handle, (long)a_Offset, SEEK_SET);

			} break;

		case std::ios_base::cur:
			{
				std::fseek(m_Handle, (long)a_Offset, SEEK_CUR);

			} break;

		case std::ios_base::end:
			{
				std::fseek(m_Handle, (long)a_Offset, SEEK_END);

			} break;
		}
		
		long offset = std::ftell(m_Handle);
		return (std::streambuf::pos_type)offset;
	}

}; // namespace ExLibris
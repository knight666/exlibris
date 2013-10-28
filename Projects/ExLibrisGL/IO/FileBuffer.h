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

#pragma once

#include <streambuf>

#include "Ownership.h"

namespace ExLibris
{

	class FileBuffer
		: public std::streambuf
	{
	
	public:
	
		FileBuffer(FILE* a_Handle, Ownership a_Ownership = eOwnership_Owned, std::size_t a_BufferSize = 256, std::size_t a_PutBackSize = 8);
		~FileBuffer();

	private:

		int_type underflow();
		pos_type seekpos(pos_type a_Position, std::ios_base::open_mode a_Mode);
		pos_type seekoff(off_type a_Offset, std::ios_base::seekdir a_Direction, std::ios_base::openmode a_Mode);

	private:

		// disallow copying
		FileBuffer(const FileBuffer& a_Other);
		FileBuffer& operator = (const FileBuffer& a_Other);

	private:

		FILE* m_Handle;
		Ownership m_Ownership;
		const std::size_t m_PutBackSize;
		char* m_Buffer;
		size_t m_BufferSize;
		pos_type m_Cursor;

	}; // class FileBuffer

}; // namespace ExLibris
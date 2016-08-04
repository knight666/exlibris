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

#include "Base.h"

namespace ExLibris {

	inline String GetFilenameFromMacroPath(const char* filePath)
	{
		const char* found = strrchr(filePath, '\\');
		if (found == nullptr)
		{
			found = strrchr(filePath, '//');
		}

		if (found != nullptr)
		{
			return String(&filePath[found - filePath + 1]);
		}
		else
		{
			return String("<unknown>");
		}
	}

#ifdef EXL_NO_EXCEPTIONS
	#define EXL_THROW(_module, _what) { std::cerr << "[" << ExLibris::GetFilenameFromMacroPath(__FILE__) << ":" << __LINE__ << "] (" << (_module) << ") " << (_what) << std::endl; }
#else
	#define EXL_THROW(_module, _what) { throw ExLibris::Exception((_module), (_what), __FILE__, __LINE__); }

	class Exception
		: public std::exception
	{
	
	public:
	
		Exception(const char* module, const char* description, const char* filePath, int line);
	
		const String& GetModule() const;

		const String& GetFilename() const;
		int GetLine() const;

	private:

		String m_Module;
		String m_Filename;
		int m_Line;
	
	};
#endif

};
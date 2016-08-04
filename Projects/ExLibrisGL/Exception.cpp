#include "ExLibrisGL.PCH.h"

#ifndef EXL_NO_EXCEPTIONS

#include "Exception.h"

namespace ExLibris {

	Exception::Exception(const char* module, const char* description, const char* filePath, int line)
		: std::exception(description)
		, m_Module(module)
		, m_Line(line)
	{
		m_Filename = GetFilenameFromMacroPath(filePath);
	}

	const String& Exception::GetModule() const
	{
		return m_Module;
	}

	const String& Exception::GetFilename() const
	{
		return m_Filename;
	}

	int Exception::GetLine() const
	{
		return m_Line;
	}

};

#endif
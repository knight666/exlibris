#include "ExLibrisGL.PCH.h"

#include "Exception.h"

namespace ExLibris
{

#ifndef EXL_NO_EXCEPTIONS

	Exception::Exception(const char* a_Module, const char* a_Description, const char* a_FilePath, int a_Line)
		: std::exception(a_Description)
		, m_Module(a_Module)
		, m_Line(a_Line)
	{
		m_Filename = GetFilenameFromMacroPath(a_FilePath);
	}

	const std::string& Exception::GetModule() const
	{
		return m_Module;
	}

	const std::string& Exception::GetFilename() const
	{
		return m_Filename;
	}

	int Exception::GetLine() const
	{
		return m_Line;
	}

#endif

}; // namespace ExLibris
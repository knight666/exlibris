#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

namespace ExLibris
{

	inline std::string GetFilenameFromMacroPath(const char* a_FilePath)
	{
		const char* found = strrchr(a_FilePath, '\\');
		if (found == nullptr)
		{
			found = strrchr(a_FilePath, '//');
		}

		if (found != nullptr)
		{
			return std::string(&a_FilePath[found - a_FilePath + 1]);
		}
		else
		{
			return std::string("<unknown>");
		}
	}

#ifdef EXL_NO_EXCEPTIONS
#	define EXL_THROW(_module, _what) { std::cerr << "[" << ExLibris::GetFilenameFromMacroPath(__FILE__) << ":" << __LINE__ << "] (" << (_module) << ") " << (_what) << std::endl; }
#else
#	define EXL_THROW(_module, _what) { throw ExLibris::Exception((_module), (_what), __FILE__, __LINE__); }

	class Exception
		: public std::exception
	{
	
	public:
	
		Exception(const char* a_Module, const char* a_Description, const char* a_FilePath, int a_Line);
	
		const std::string& GetModule() const;

		const std::string& GetFilename() const;
		int GetLine() const;

	private:

		std::string m_Module;
		std::string m_Filename;
		int m_Line;
	
	}; // class Exception

#endif // EXL_NO_EXCEPTIONS

}; // namespace ExLibris

#endif
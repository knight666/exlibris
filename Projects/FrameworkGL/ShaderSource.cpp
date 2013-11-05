#include "FrameworkGL.PCH.h"

#include "ShaderSource.h"

namespace Framework
{

	ShaderSource::ShaderSource(const std::string& a_FileName, GLenum a_Target)
		: m_FileName(a_FileName)
		, m_Target(a_Target)
		, m_Handle(0)
		, m_Compiled(false)
		, m_Log(nullptr)
		, m_LogLength(0)
	{
		m_Handle = glCreateShader(m_Target);
	}

	ShaderSource::ShaderSource(ShaderProgram* a_Program, GLenum a_Target)
		: m_Program(a_Program)
		, m_Target(a_Target)
		, m_Handle(0)
		, m_Compiled(false)
		, m_Log(nullptr)
		, m_LogLength(0)
	{
		m_Handle = glCreateShader(m_Target);
	}

	ShaderSource::~ShaderSource()
	{
		glDeleteShader(m_Handle);

		if (m_Log != nullptr)
		{
			delete [] m_Log;
		}
	}

	bool ShaderSource::Compile(const std::string& a_Source)
	{
		const GLchar* source = a_Source.c_str();
		glShaderSource(m_Handle, 1, &source, nullptr);
		glCompileShader(m_Handle);

		m_LogLength = 0;
		glGetShaderiv(m_Handle, GL_INFO_LOG_LENGTH, &m_LogLength);
		if (m_LogLength > 1)
		{
			if (m_Log != nullptr)
			{
				delete [] m_Log;
				m_Log = nullptr;
			}
			m_Log = new GLchar[m_LogLength + 1];
			glGetShaderInfoLog(m_Handle, m_LogLength, 0, m_Log);

			// some video card manufacturers (Intel) like to use the log to
			// output a message explicitly saying that there were... no errors.
			// we skip this message because it's specific to this manufacturer
			// and it's never ever going to be useful information

			if (!strcmp(m_Log, "No errors.\n"))
			{
				delete [] m_Log;
				m_Log = nullptr;
			}
		}

		GLint success = GL_TRUE;
		glGetShaderiv(m_Handle, GL_COMPILE_STATUS, &success);

		return (success == GL_TRUE);
	}

	void ShaderSource::Load(const std::string& a_Source)
	{
		m_Source = a_Source;
	}

	bool ShaderSource::IsLoaded() const
	{
		return (m_Source.length() > 0);
	}

	void ShaderSource::Compile()
	{
		m_Compiled = false;

		if (!IsLoaded())
		{
			return;
		}

		if (m_Handle == 0)
		{
			m_Handle = glCreateShader(m_Target);
		}

		const GLchar* source = m_Source.c_str();
		glShaderSource(m_Handle, 1, &source, nullptr);
		glCompileShader(m_Handle);

		m_LogLength = 0;
		glGetShaderiv(m_Handle, GL_INFO_LOG_LENGTH, &m_LogLength);
		if (m_LogLength > 1)
		{
			if (m_Log != nullptr)
			{
				delete [] m_Log;
				m_Log = nullptr;
			}
			m_Log = new GLchar[m_LogLength + 1];
			glGetShaderInfoLog(m_Handle, m_LogLength, 0, m_Log);

			// some video card manufacturers (Intel) like to use the log to
			// output a message explicitly saying that there were... no errors.
			// we skip this message because it's specific to this manufacturer
			// and it's never ever going to be useful information

			if (!strcmp(m_Log, "No errors.\n"))
			{
				delete [] m_Log;
				m_Log = nullptr;
			}
		}

		GLint success;
		glGetShaderiv(m_Handle, GL_COMPILE_STATUS, &success);
		m_Compiled = (success == GL_TRUE);
	}

	bool ShaderSource::IsCompiled() const
	{
		return m_Compiled;
	}

	const GLchar* ShaderSource::GetLog() const
	{
		return m_Log;
	}

	GLuint ShaderSource::GetHandle() const
	{
		return m_Handle;
	}

	std::string ShaderSource::GetLogString() const
	{
		if (m_Log == nullptr)
		{
			return "";
		}

		return std::string(m_Log, m_Log + m_LogLength);
	}

}; // namespace Framework
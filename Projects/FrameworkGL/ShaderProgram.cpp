#include "FrameworkGL.PCH.h"

#include "ShaderProgram.h"

#include "ShaderSource.h"

namespace Framework
{

	ShaderProgram::ShaderProgram(const std::string& a_Name)
		: m_Handle(0)
		, m_Name(a_Name)
		, m_SourceVertex(nullptr)
		, m_SourceGeometry(nullptr)
		, m_SourceFragment(nullptr)
		, m_Log(nullptr)
		, m_Loaded(false)
		, m_Compiled(false)
		, m_Linked(false)
	{
		m_Handle = glCreateProgram();
	}
	
	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_Handle);

		if (m_Log != nullptr)
		{
			delete [] m_Log;
		}

		if (m_SourceVertex != nullptr)
		{
			delete m_SourceVertex;
		}
		if (m_SourceFragment != nullptr)
		{
			delete m_SourceFragment;
		}
		if (m_SourceGeometry != nullptr)
		{
			delete m_SourceGeometry;
		}
	}

	GLuint ShaderProgram::GetHandle() const
	{
		return m_Handle;
	}

	const std::string& ShaderProgram::GetName() const
	{
		return m_Name;
	}

	void ShaderProgram::LoadSource(GLenum a_Target, const std::string& a_Source)
	{
		ShaderSource* source = nullptr;
		std::string target_name = "";

		switch (a_Target)
		{

		case GL_VERTEX_SHADER:
			{
				if (m_SourceVertex == nullptr)
				{
					m_SourceVertex = new ShaderSource(this, GL_VERTEX_SHADER);
					glAttachShader(m_Handle, m_SourceVertex->GetHandle());
				}

				source = m_SourceVertex;
				target_name = "vertex";

			} break;

		case GL_FRAGMENT_SHADER:
			{
				if (m_SourceFragment == nullptr)
				{
					m_SourceFragment = new ShaderSource(this, GL_FRAGMENT_SHADER);
					glAttachShader(m_Handle, m_SourceFragment->GetHandle());
				}

				source = m_SourceFragment;
				target_name = "fragment";

			} break;

		case GL_GEOMETRY_SHADER:
			{
				if (m_SourceGeometry == nullptr)
				{
					m_SourceGeometry = new ShaderSource(this, GL_GEOMETRY_SHADER);
					glAttachShader(m_Handle, m_SourceGeometry->GetHandle());
				}

				source = m_SourceGeometry;
				target_name = "geometry";

			} break;

		default:
			{
				std::stringstream ss;
				ss << "Unknown shader target 0x" << std::hex << a_Target << "." << std::endl;

				throw std::exception(ss.str().c_str());

			} break;
		}

		if (!source->Compile(a_Source))
		{
			std::stringstream ss;
			ss << "Error while compiling " << target_name << " shader." << std::endl << std::endl << source->GetLogString() << std::endl;

			throw std::exception(ss.str().c_str());
		}
	}

	void ShaderProgram::LoadSourceFromFile(GLenum a_Target, const std::string& a_FilePath)
	{
		std::fstream file_handle(a_FilePath);
		if (!file_handle.is_open())
		{
			std::stringstream ss;
			ss << "Could not load shader file: " << a_FilePath << std::endl;

			throw std::exception(ss.str().c_str());
		}

		std::stringstream ss;
		ss << file_handle.rdbuf();
		file_handle.close();

		LoadSource(a_Target, ss.str());
	}

	void ShaderProgram::SetSourceVertex(ShaderSource* a_Source)
	{
		m_SourceVertex = a_Source;
	}

	void ShaderProgram::SetSourceGeometry(ShaderSource* a_Source)
	{
		m_SourceGeometry = a_Source;
	}

	void ShaderProgram::SetSourceFragment(ShaderSource* a_Source)
	{
		m_SourceFragment = a_Source;
	}

	GLint ShaderProgram::GetAttribute(const std::string& a_Name)
	{
		GLint result = -1;

		std::map<std::string, GLint>::const_iterator found = m_CacheAttributes.find(a_Name);
		if (found != m_CacheAttributes.end())
		{
			result = found->second;
		}

		return result;
	}

	GLint ShaderProgram::GetUniform(const std::string& a_Name)
	{
		GLint result = -1;

		std::map<std::string, GLint>::const_iterator found = m_CacheUniforms.find(a_Name);
		if (found != m_CacheUniforms.end())
		{
			result = found->second;
		}

		return result;
	}

	void ShaderProgram::Compile()
	{
		std::cout << "Compiling '" << m_Name << "'..." << std::endl;

		if (
			!m_SourceVertex || !m_SourceVertex->IsLoaded() || 
			!m_SourceFragment || !m_SourceFragment->IsLoaded()
		)
		{
			m_Compiled = false;
			return;
		}

		m_Compiled = true;

		// geometry

		if (m_SourceGeometry && m_SourceGeometry->IsLoaded())
		{
			m_SourceGeometry->Compile();

			m_Compiled = m_Compiled && m_SourceGeometry->IsCompiled();
			if (!m_SourceGeometry->IsCompiled())
			{
				std::cerr << "Failed to compile geometry shader." << std::endl;
			}

			if (m_SourceGeometry->GetLog())
			{
				std::cout << m_SourceGeometry->GetLog() << std::endl;
			}
		}

		// vertex

		m_SourceVertex->Compile();

		m_Compiled = m_Compiled && m_SourceVertex->IsCompiled();
		if (!m_SourceVertex->IsCompiled())
		{
			std::cerr << "Failed to compile vertex shader." << std::endl;
		}

		if (m_SourceVertex->GetLog())
		{
			std::cout << m_SourceVertex->GetLog() << std::endl;
		}

		// fragment

		m_SourceFragment->Compile();

		m_Compiled = m_Compiled && m_SourceFragment->IsCompiled();
		if (!m_SourceFragment->IsCompiled())
		{
			std::cerr << "Failed to compile fragment shader." << std::endl;
		}

		if (m_SourceFragment->GetLog())
		{
			std::cout << m_SourceFragment->GetLog() << std::endl;
		}
	}

	bool ShaderProgram::IsCompiled() const
	{
		return m_Compiled;
	}

	void ShaderProgram::Link()
	{
		if (m_SourceVertex == nullptr || m_SourceFragment == nullptr)
		{
			std::stringstream ss;
			ss << "Could not link shader program due to missing ";

			if (m_SourceVertex == nullptr)
			{
				ss << "vertex";
			}
			else if (m_SourceFragment == nullptr)
			{
				ss << "fragment";
			}

			ss << " shader.";

			throw std::exception(ss.str().c_str());
		}

		glLinkProgram(m_Handle);

		m_LogLength = 0;
		glGetProgramiv(m_Handle, GL_INFO_LOG_LENGTH, &m_LogLength);
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
		glGetProgramiv(m_Handle, GL_LINK_STATUS, &success);
		if (success != GL_TRUE)
		{
			std::stringstream ss;
			ss << "Error while linking program." << std::endl << std::endl << GetLog() << std::endl;

			throw std::exception(ss.str().c_str());
		}

		glValidateProgram(m_Handle);

		glGetProgramiv(m_Handle, GL_VALIDATE_STATUS, &success);
		if (success != GL_TRUE)
		{
			std::stringstream ss;
			ss << "Error while validating program." << std::endl << std::endl << GetLog() << std::endl;

			throw std::exception(ss.str().c_str());
		}

		GLchar* property_name = nullptr;
		GLsizei property_name_length = 0;
		GLint property_name_length_maximum = 0;
		GLint property_count = 0;
		GLenum property_type = 0;
		GLint property_size = 0;

		// attributes

		glGetProgramiv(m_Handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &property_name_length_maximum);
		glGetProgramiv(m_Handle, GL_ACTIVE_ATTRIBUTES, &property_count);

		property_name = new GLchar[property_name_length_maximum];

		m_CacheAttributes.clear();

		for (GLint property_index = 0; property_index < property_count; ++property_index)
		{
			glGetActiveAttrib(
				m_Handle,
				property_index,
				property_name_length_maximum,
				&property_name_length,
				&property_size,
				&property_type,
				property_name
			);

			GLint attribute_location = glGetAttribLocation(m_Handle, property_name);

			m_CacheAttributes[property_name] = attribute_location;
		}

		delete [] property_name;

		// uniforms

		glGetProgramiv(m_Handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &property_name_length_maximum);
		glGetProgramiv(m_Handle, GL_ACTIVE_UNIFORMS, &property_count);

		property_name = new GLchar[property_name_length_maximum];

		m_CacheUniforms.clear();

		for (GLint property_index = 0; property_index < property_count; ++property_index)
		{
			glGetActiveUniform(
				m_Handle,
				property_index,
				property_name_length_maximum,
				&property_name_length,
				&property_size,
				&property_type,
				property_name
			);

			// skim array brackets off of uniform name for cross-driver compatibility

			if (property_size > 1 && property_name_length > 3)
			{
				char* uniform_bracket = strrchr(property_name, '[');
				if (uniform_bracket)
				{
					int uniform_bracket_length = strlen(property_name) - (uniform_bracket - property_name);
					memset(uniform_bracket, 0, uniform_bracket_length);
				}
			}

			GLint uniform_location = glGetUniformLocation(m_Handle, property_name);

			m_CacheUniforms[property_name] = uniform_location;
		}

		delete [] property_name;
	}

	bool ShaderProgram::IsLinked() const
	{
		return m_Linked;
	}

	std::string ShaderProgram::GetLog() const
	{
		if (m_Log == nullptr)
		{
			return "";
		}

		return std::string(m_Log, m_Log + m_LogLength);
	}

}; // namespace Framework
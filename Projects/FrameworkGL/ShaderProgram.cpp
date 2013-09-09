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
	}
	
	ShaderProgram::~ShaderProgram()
	{
	}

	GLuint ShaderProgram::GetHandle() const
	{
		return m_Handle;
	}

	const std::string& ShaderProgram::GetName() const
	{
		return m_Name;
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
		if (!IsCompiled())
		{
			m_Linked = false;
			return;
		}

		if (
			!m_SourceVertex || !m_SourceVertex->IsCompiled() || 
			!m_SourceFragment || !m_SourceFragment->IsCompiled()
		)
		{
			m_Linked = false;
			return;
		}

		std::cout << "Linking '" << m_Name << "'..." << std::endl;

		m_Linked = true;

		if (m_Handle == 0)
		{
			m_Handle = glCreateProgram();
		}

		glAttachShader(m_Handle, m_SourceVertex->GetHandle());
		glAttachShader(m_Handle, m_SourceFragment->GetHandle());

		if (m_SourceGeometry && m_SourceGeometry->IsCompiled())
		{
			glAttachShader(m_Handle, m_SourceGeometry->GetHandle());
		}
		
		glLinkProgram(m_Handle);

		GLint info_log_length;
		glGetProgramiv(m_Handle, GL_INFO_LOG_LENGTH, &info_log_length);
		if (info_log_length > 1)
		{
			m_Log = new GLchar[info_log_length + 1];
			glGetProgramInfoLog(m_Handle, info_log_length, 0, m_Log);
			if (strcmp(m_Log, "No errors."))
			{
				delete [] m_Log;
				m_Log = nullptr;
			}
		}

		GLint success;
		glGetProgramiv(m_Handle, GL_LINK_STATUS, &success);
		m_Linked = m_Linked && (success == GL_TRUE);
		if (!m_Linked)
		{
			return;
		}

		glValidateProgram(m_Handle);
		glGetProgramiv(m_Handle, GL_VALIDATE_STATUS, &success);
		m_Linked = m_Linked && (success == GL_TRUE);
		if (!m_Linked)
		{
			return;
		}

		GLchar* property_name;
		GLsizei property_name_length;
		GLint property_name_length_maximum;
		GLint property_count;
		GLenum property_type;
		GLint property_size;

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

			std::cout << "Attribute " << property_index << " - " << property_name << " (" << attribute_location << ")" << std::endl;

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

			std::cout << "Uniform " << property_index << " - " << property_name << " (" << uniform_location << ")" << std::endl;

			m_CacheUniforms[property_name] = uniform_location;
		}

		delete [] property_name;
	}

	bool ShaderProgram::IsLinked() const
	{
		return m_Linked;
	}

}; // namespace Framework
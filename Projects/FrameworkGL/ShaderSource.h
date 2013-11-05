#ifndef _SHADERSOURCE_H_
#define _SHADERSOURCE_H_

namespace Framework
{
	class ShaderProgram;
}

namespace Framework
{

	class ShaderSource
	{

	public:

		ShaderSource(ShaderProgram* a_Program, GLenum a_Target);
		~ShaderSource();

		GLuint GetHandle() const;

		std::string GetLog() const;

		bool Compile(const std::string& a_Source);

	private:

		ShaderProgram* m_Program;

		GLuint m_Handle;
		GLenum m_Target;

		GLchar* m_Log;
		GLint m_LogLength;

	}; // class ShaderSource

}; // namespace Framework

#endif
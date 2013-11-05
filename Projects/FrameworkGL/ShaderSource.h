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

		ShaderSource(const std::string& a_FileName, GLenum a_Target);
		ShaderSource(ShaderProgram* a_Program, GLenum a_Target);
		~ShaderSource();

		GLuint GetHandle() const;

		bool Compile(const std::string& a_Source);

		void Load(const std::string& a_Source);
		bool IsLoaded() const;

		void Compile();
		bool IsCompiled() const;

		const GLchar* GetLog() const;

		std::string GetLogString() const;

	private:

		ShaderProgram* m_Program;

		GLuint m_Handle;
		GLenum m_Target;
		std::string m_FileName;

		std::string m_Source;

		GLchar* m_Log;
		GLint m_LogLength;
		bool m_Compiled;

	}; // class ShaderSource

}; // namespace Framework

#endif
#ifndef _SHADERSOURCE_H_
#define _SHADERSOURCE_H_

namespace Framework
{

	class ShaderSource
	{

	public:

		ShaderSource(const std::string& a_FileName, GLenum a_Target);
		~ShaderSource();

		void Load(const std::string& a_Source);
		bool IsLoaded() const;

		void Compile();
		bool IsCompiled() const;

		const GLchar* GetLog() const;

		GLuint GetHandle() const;

	private:

		GLuint m_Handle;
		GLenum m_Target;
		std::string m_FileName;

		std::string m_Source;

		GLchar* m_Log;
		bool m_Compiled;

	}; // class ShaderSource

}; // namespace Framework

#endif
#ifndef _SHADERPROGRAM_H_
#define _SHADERPROGRAM_H_

#include "ShaderSource.h"

namespace Framework
{

	class ShaderProgram
	{

	public:

		ShaderProgram(const std::string& a_Name);
		~ShaderProgram();

		GLuint GetHandle() const;

		const std::string& GetName() const;

		void LoadSource(GLenum a_Target, const std::string& a_Source);
		void LoadSourceFromFile(GLenum a_Target, const std::string& a_FilePath);

		void SetSourceVertex(ShaderSource* a_Source);
		void SetSourceGeometry(ShaderSource* a_Source);
		void SetSourceFragment(ShaderSource* a_Source);

		GLint GetAttribute(const std::string& a_Name);

		GLint GetUniform(const std::string& a_Name);

		void Compile();
		bool IsCompiled() const;

		void Link();
		bool IsLinked() const;

		std::string GetLog() const;

	private:

		GLuint m_Handle;
		std::string m_Name;

		ShaderSource* m_SourceVertex;
		ShaderSource* m_SourceGeometry;
		ShaderSource* m_SourceFragment;

		GLchar* m_Log;
		GLint m_LogLength;
		bool m_Loaded;
		bool m_Compiled;
		bool m_Linked;

		std::map<std::string, GLint> m_CacheAttributes;
		std::map<std::string, GLint> m_CacheUniforms;

	}; // class ShaderProgram

}; // namespace Framework

#endif
#ifndef _SHADERPROGRAM_H_
#define _SHADERPROGRAM_H_

#include "ShaderSource.h"

namespace Framework
{

	class ShaderProgram
	{

	public:

		static ShaderProgram* Create(
			const std::string& a_VertexSource,
			const std::string& a_FragmentSource,
			const std::string& a_GeometrySource = ""
		);
		static ShaderProgram* CreateFromFile(
			const std::string& a_VertexFile,
			const std::string& a_FragmentFile,
			const std::string& a_GeometryFile = ""
		);

	public:

		ShaderProgram();
		~ShaderProgram();

		GLuint GetHandle() const;

		std::string GetLog() const;

		void LoadSource(GLenum a_Target, const std::string& a_Source);
		void LoadSourceFromFile(GLenum a_Target, const std::string& a_FilePath);

		GLint FindAttribute(const std::string& a_Name) const;

		GLint FindUniform(const std::string& a_Name) const;

		void Link();

	private:

		GLuint m_Handle;

		ShaderSource* m_SourceVertex;
		ShaderSource* m_SourceGeometry;
		ShaderSource* m_SourceFragment;

		GLchar* m_Log;
		GLint m_LogLength;

		std::map<std::string, GLint> m_CacheAttributes;
		std::map<std::string, GLint> m_CacheUniforms;

	}; // class ShaderProgram

}; // namespace Framework

#endif
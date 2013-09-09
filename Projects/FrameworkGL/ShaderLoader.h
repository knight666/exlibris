#ifndef _SHADERLOADER_H_
#define _SHADERLOADER_H_

namespace Framework
{
	class ShaderProgram;
	class ShaderSource;
}

namespace Framework
{

	class ShaderLoader
	{

	public:

		ShaderSource* LoadSource(const std::string& a_Path, GLenum a_Type);

		ShaderProgram* LoadProgram(const std::string& a_Name, const std::string& a_Path);

	}; // class ShaderLoader

}; // namespace Framework

#endif
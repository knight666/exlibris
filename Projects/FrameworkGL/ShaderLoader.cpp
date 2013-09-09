#include "FrameworkGL.PCH.h"

#include "ShaderLoader.h"

#include "ShaderProgram.h"
#include "ShaderSource.h"

namespace Framework
{

	ShaderSource* ShaderLoader::LoadSource(const std::string& a_Path, GLenum a_Type)
	{
		ShaderSource* result = nullptr;

		std::fstream file_handle(a_Path);
		if (!file_handle.is_open())
		{
			return result;
		}

		std::stringstream ss;
		ss << file_handle.rdbuf();
		file_handle.close();

		size_t last_slash = a_Path.find_first_of('/');
		if (last_slash == std::string::npos)
		{
			last_slash = a_Path.find_first_of('\\');
		}

		if (last_slash == std::string::npos)
		{
			result = new ShaderSource(a_Path, a_Type);
		}
		else
		{
			std::string filename = a_Path.substr(last_slash + 1, a_Path.length() - last_slash - 1);
			result = new ShaderSource(filename, a_Type);
		}

		result->Load(ss.str());

		return result;
	}

	ShaderProgram* ShaderLoader::LoadProgram(const std::string& a_Name, const std::string& a_Path)
	{
		ShaderProgram* result = nullptr;

		ShaderSource* source_vertex = LoadSource(a_Path + ".vert", GL_VERTEX_SHADER);
		ShaderSource* source_fragment = LoadSource(a_Path + ".frag", GL_FRAGMENT_SHADER);

		if (source_vertex && source_fragment)
		{
			result = new ShaderProgram(a_Name);
			result->SetSourceVertex(source_vertex);
			result->SetSourceFragment(source_fragment);

			ShaderSource* source_geometry = LoadSource(a_Path + ".geom", GL_GEOMETRY_SHADER);
			if (source_geometry)
			{
				result->SetSourceGeometry(source_geometry);
			}
		}

		return result;
	}

}; // namespace Framework
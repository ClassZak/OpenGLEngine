#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <cctype>
#include <utility>
#include <string>

#include "../utils/functions.hpp"

class Shader
{
private:
	std::map<std::string, std::string> m_uniforms;
	std::string m_fragmentShader;
	std::string m_vertexShader;
	
	bool m_hasFragment = false;
	bool m_hasVertex = false;
	
	Shader()
	{ }
public:
	Shader(const std::string filepath) : Shader()
	{
		std::string source = LoadDataFromFile(filepath);
		GetVertexAndFragmentShaders(*this, source);
	}
private:

	// TODO переделать
	static inline void GetVertexAndFragmentShaders(Shader& shader, const std::string& source)
	{
		const std::string vertexLabel = "#shader vertex";
		const std::string fragmentLabel = "#shader fragment";
		const std::size_t vertexLabelSize = vertexLabel.size();
		const std::size_t fragmentLabelSize = fragmentLabel.size();
		const std::size_t vertexLabelPos = source.find(vertexLabel);
		const std::size_t fragmentLabelPos = source.find(fragmentLabel);
		
		shader.m_hasVertex = vertexLabelPos != std::string::npos;
		shader.m_hasFragment = fragmentLabelPos != std::string::npos;
		if (shader.m_hasVertex)
		{
			const std::size_t start =
			vertexLabelPos + vertexLabelSize,
			length= 
				shader.m_hasFragment and
				fragmentLabelPos > vertexLabelPos ?
				(fragmentLabelPos - start) : std::string::npos;
			
			shader.m_vertexShader = source.substr(start, length);
		}
		if (shader.m_hasFragment)
		{
			const std::size_t start = 
			fragmentLabelPos + fragmentLabelSize,
			length = 
				shader.m_hasVertex and vertexLabelPos > fragmentLabelPos ?
				(fragmentLabelPos - start) : std::string::npos;
			
			shader.m_fragmentShader = source.substr(start, length);
		}
	}



	/// <summary>
	/// Компиляция шейдера
	/// </summary>
	/// <param name="type"></param>
	/// <param name="source"></param>
	/// <returns>Id шейдера. 0 - ошибка</returns>
	static GLuint CompileShader(GLuint type, const std::string& source);

	/// <summary>
	/// Функция создания шейдера
	/// </summary>
	/// <param name="vertexShader"></param>
	/// <param name="fragmentShader"></param>
	/// <returns>Id программы. 0 - ошибка</returns>
	static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};

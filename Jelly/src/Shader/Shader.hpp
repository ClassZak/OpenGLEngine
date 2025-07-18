#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#include "../utils/functions.hpp"

struct Shader
{
	std::string m_fragmentShader;
	std::string m_vertexShader;
	
	bool m_hasFragment = false;
	bool m_hasVertex = false;
	



	// TODO переделать
	static inline struct Shader GetVertexAndFragmentShaders(const std::string& source)
	{
		Shader shader;

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

		return shader;
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

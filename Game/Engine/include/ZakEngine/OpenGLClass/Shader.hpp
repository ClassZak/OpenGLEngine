#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <list>
#include <unordered_map>
#include <sstream>
#include <cctype>
#include <utility>
#include <array>
#include <vector>
#include <variant>
#include <cstdint>
#include <regex>
#include <glm/glm.hpp>

#include "../utils/functions.hpp"
#include "../utils/GLMacro.h"
#include "Uniform.hpp"

namespace Zak {
class Shader
{
private:
	std::unordered_map<std::string, int> m_uniformLocationCache;
	std::string m_fragmentShader;
	std::string m_vertexShader;
	
	GLuint m_program{0};
	bool m_hasFragment = false;
	bool m_hasVertex = false;

public:
	Shader(const std::string& filepath)
	{
		std::string source = LoadDataFromFile(filepath);
		Shader::GetVertexAndFragmentShaders(*this, source);

		GLuint program;
		GLuint fragmentShader=0, vertexShader=0;
		if(m_hasFragment)
			fragmentShader = Shader::CompileShader(GL_FRAGMENT_SHADER, m_fragmentShader);
		if(m_hasVertex)
			vertexShader = Shader::CompileShader(GL_VERTEX_SHADER, m_vertexShader);

		GL_ASSERT(fragmentShader!= 0 and vertexShader!= 0);

		GL_ASSERT(m_program = Shader::CreateShader(m_vertexShader, m_fragmentShader));
	}
	~Shader()
	{
		glDeleteProgram(m_program);
	}

	void Bind() const
	{
		GL_ASSERT(m_program);
		glUseProgram(m_program);
	}
	void UnBind() const
	{
		glUseProgram(0);
	}

	GLint GetUniformLocation(const std::string& uniform);
	GLint GetProgram()const
	{
		return m_program;
	}
	


	void SetUniform(const Uniform& uniform);



	bool operator == (const Shader& other) const;
	bool operator != (const Shader& other) const;
	bool operator >  (const Shader& other) const;
	bool operator <  (const Shader& other) const;
	bool operator <= (const Shader& other) const;
	bool operator >= (const Shader& other) const;

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
}
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

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

		if (vertexLabelPos != std::string::npos)
		{
			shader.m_hasVertex = true;

			shader.m_vertexShader =
				source.substr
				(
					vertexLabelPos + vertexLabelSize,
					source.size() -
					(
						fragmentLabelPos != std::string::npos and
						fragmentLabelPos > vertexLabelPos ?
						(fragmentLabelPos + fragmentLabelSize + 1) : std::string::npos
					)
				);
		}
		if (fragmentLabelPos != std::string::npos)
		{
			shader.m_hasFragment = true;

			shader.m_fragmentShader =
				source.substr
				(
					fragmentLabelPos + fragmentLabelSize,
					source.size() -
					(
						vertexLabelPos != std::string::npos and
						vertexLabelPos > fragmentLabelPos ?
						(vertexLabelPos + vertexLabelSize + 1) : std::string::npos
					)
				);
		}

		return shader;
	}



	/// <summary>
	/// Компиляция шейдера
	/// </summary>
	/// <param name="type"></param>
	/// <param name="source"></param>
	/// <returns>Id шейдера. 0 - ошибка</returns>
	static GLuint CompileShader(GLuint type, const std::string& source)
	{
		GLuint id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		// TODO: обработка ошибок
		GLint result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			// alloca для динамического наполнения стека
			char* message = (char*)_malloca(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			std::cout << "Failed to compile " <<
				(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
			std::cout << message << std::endl;
			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	/// <summary>
	/// Функция создания шейдера
	/// </summary>
	/// <param name="vertexShader"></param>
	/// <param name="fragmentShader"></param>
	/// <returns>Id программы. 0 - ошибка</returns>
	static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		GLuint program = glCreateProgram();
		GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		if (!vs)
		{
			std::cout << "Не удалось создать вершинный шейдер" << std::endl;
			return 0;
		}
		GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
		if (!fs)
		{
			std::cout << "Не удалось создать фрагментный шейдер" << std::endl;
			return 0;
		}

		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		glValidateProgram(program);

		glDeleteShader(vs);
		glDeleteShader(fs);


		return program;
	}
};

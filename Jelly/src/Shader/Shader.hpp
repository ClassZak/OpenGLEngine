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
	/// Удаление комментариев из исходного кода шейдера
	/// </summary>
	static inline std::string RemoveComments(const std::string& source)
	{
		std::string result;
		result.reserve(source.size());

		bool in_single_line = false;
		bool in_multi_line = false;

		for (size_t i = 0; i < source.size(); ++i)
		{
			// Обработка многострочных комментариев
			if (in_multi_line)
			{
				if (source[i] == '*' && i + 1 < source.size() && source[i + 1] == '/')
				{
					in_multi_line = false;
					++i; // Пропускаем '/'
				}
				continue;
			}

			// Обработка однострочных комментариев
			if (in_single_line)
			{
				if (source[i] == '\n') {
					in_single_line = false;
					result += '\n'; // Сохраняем перевод строки
				}
				continue;
			}

			// Проверка начала комментариев
			if (source[i] == '/')
			{
				if (i + 1 < source.size()) {
					if (source[i + 1] == '/') {
						in_single_line = true;
						++i; // Пропускаем второй '/'
						continue;
					}
					else if (source[i + 1] == '*') {
						in_multi_line = true;
						++i; // Пропускаем '*'
						continue;
					}
				}
			}
			// Сохраняем символ, если не в комментарии
			result += source[i];
		}

		return result;
	}
	/// <summary>
	/// Структура для хранения информации об униформе
	/// </summary>
	struct Uniform
	{
		std::string name;
		std::string type;
	};
	/// <summary>
	/// Извлечение информации об униформах из исходного кода шейдера
	/// </summary>
	/// <returns>Список униформ в порядке их объявления</returns>
	static inline std::list<Uniform> GetUniforms(const std::string& source)
	{
		std::list<Uniform> uniforms;
		std::string clean_source = RemoveComments(source);
		std::istringstream iss(clean_source);
		std::string line;

		while (std::getline(iss, line))
		{
			// Поиск ключевого слова "uniform"
			size_t uniform_pos = line.find("uniform");
			if (uniform_pos == std::string::npos) continue;

			// Пропускаем само слово "uniform"
			size_t type_start = uniform_pos + 7; // 7 = strlen("uniform")

			// Пропускаем пробелы после "uniform"
			while (type_start < line.size() && std::isspace(line[type_start]))
				++type_start;

			if (type_start >= line.size()) continue;

			// Извлечение типа униформы
			size_t type_end = type_start;
			while (type_end < line.size() && (std::isalnum(line[type_end]) || line[type_end] == '_'))
				++type_end;
			if (type_end == type_start) continue;
			std::string uniform_type = line.substr(type_start, type_end - type_start);

			// Пропускаем пробелы после типа
			size_t name_start = type_end;
			while (name_start < line.size() && std::isspace(line[name_start]))
				++name_start;
			if (name_start >= line.size()) continue;

			// Извлечение имени униформы
			size_t name_end = name_start;
			while (name_end < line.size() && (std::isalnum(line[name_end]) || line[name_end] == '_'))
				++name_end;
			if (name_end == name_start) continue;
			std::string uniform_name = line.substr(name_start, name_end - name_start);

			// Добавляем униформу в список
			uniforms.push_back({ std::move(uniform_name), std::move(uniform_type) });
		}

		return uniforms;
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

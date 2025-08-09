#include "Shader.hpp"

GLint Shader::GetUniformLocation(const std::string& uniform)
{
	bool notCached = m_uniformLocationCache.find(uniform) == m_uniformLocationCache.end();
	GLint location = notCached ? 
	glGetUniformLocation(m_program, uniform.c_str()) : m_uniformLocationCache[uniform];

	if(location == -1)
		std::cout<<"Failed to locate uniform \""<<uniform<<"\""<<std::endl;
	else if(notCached)
		m_uniformLocationCache[uniform] = location;

	return location;
}




void Shader::SetUniform(const Uniform& uniform)
{
	int location = GetUniformLocation(uniform.m_name);
#ifdef DEBUG_MODE
	GL_ASSERT(location + 1);
#endif
	std::visit
	(
		[location](auto && arg)
		{
			using T = std::decay_t<decltype(arg)>;

			// Скалярные значения
			if constexpr (std::is_same_v<T, UniformInt> ||
				std::is_same_v<T, UniformSampler>)
				glUniform1i(location, arg);
			else if constexpr (std::is_same_v<T, UniformFloat>)
				glUniform1f(location, arg);
			else if constexpr (std::is_same_v<T, UniformBool>)
				glUniform1i(location, arg ? 1 : 0);

			// Векторы
			else if constexpr (std::is_same_v<T, UniformVec2>)
				glUniform2fv(location, 1, &arg[0]);
			else if constexpr (std::is_same_v<T, UniformVec3>)
				glUniform3fv(location, 1, &arg[0]);
			else if constexpr (std::is_same_v<T, UniformVec4>)
				glUniform4fv(location, 1, &arg[0]);

			// Матрицы
			else if constexpr (std::is_same_v<T, UniformMat3>)
				glUniformMatrix3fv(location, 1, GL_FALSE, &arg[0][0]);
			else if constexpr (std::is_same_v<T, UniformMat4>)
				glUniformMatrix4fv(location, 1, GL_FALSE, &arg[0][0]);

			// Массивы скаляров
			else if constexpr (std::is_same_v<T, UniformIntArray> ||
				std::is_same_v<T, UniformSamplerArray>)
				glUniform1iv(location, static_cast<GLsizei>(arg.size()), arg.data());
			else if constexpr (std::is_same_v<T, UniformFloatArray>)
				glUniform1fv(location, static_cast<GLsizei>(arg.size()), arg.data());
			else if constexpr (std::is_same_v<T, UniformBoolArray>)
			{
				std::vector<int> ints;
				ints.reserve(arg.size());
				for (bool b : arg) ints.push_back(b ? 1 : 0);
				glUniform1iv(location, static_cast<GLsizei>(ints.size()), ints.data());
			}

			// Массивы векторов
			else if constexpr (std::is_same_v<T, UniformVec2Array>)
				glUniform2fv(location, static_cast<GLsizei>(arg.size()), (GLfloat*)(arg.data()));
			else if constexpr (std::is_same_v<T, UniformVec3Array>)
				glUniform3fv(location, static_cast<GLsizei>(arg.size()), (GLfloat*)(arg.data()));
			else if constexpr (std::is_same_v<T, UniformVec4Array>)
				glUniform4fv(location, static_cast<GLsizei>(arg.size()), (GLfloat*)(arg.data()));

			// Массивы матриц
			else if constexpr (std::is_same_v<T, UniformMat3Array>)
				glUniformMatrix3fv(location, static_cast<GLsizei>(arg.size()),
					GL_FALSE, (GLfloat*)(arg.data()));
			else if constexpr (std::is_same_v<T, UniformMat4Array>)
				glUniformMatrix4fv(location, static_cast<GLsizei>(arg.size()),
					GL_FALSE, (GLfloat*)(arg.data()));
		},
		uniform.m_value
	);
}




#pragma region
bool Shader::operator==(const Shader& other)const
{
	if(&other==this)
		return true;

	return GetProgram() == other.GetProgram();
}
bool Shader::operator!=(const Shader& other)const
{
	return !(*this==other);
}
bool Shader::operator>(const Shader& other)const
{
	if (&other == this)
		return false;

	return GetProgram() > other.GetProgram();
}
bool Shader::operator<(const Shader& other)const
{
	if (&other == this)
		return false;

	return GetProgram() < other.GetProgram();
}
bool Shader::operator>=(const Shader& other)const
{
	if (&other == this)
		return true;

	return GetProgram() >= other.GetProgram();
}
bool Shader::operator<=(const Shader& other)const
{
	if (&other == this)
		return true;

	return GetProgram() <= other.GetProgram();
}
#pragma endregion


GLuint Shader::CompileShader(GLuint type, const std::string& source)
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
		char* message = (char*)malloc(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);

		return 0;
	}

	return id;
}

GLuint Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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


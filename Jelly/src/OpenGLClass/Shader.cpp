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

void Shader::SetUniform_4(const std::string& uniform, float v0, float v1, float v2, float v3)
{
	int location = GetUniformLocation(uniform);
#ifdef DEBUG_MODE
	GL_ASSERT(location + 1);
#endif
	glUniform4f(location, v0, v1, v2, v3);
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


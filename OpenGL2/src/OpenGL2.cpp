#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>

#include <iostream>
#include <locale>
#include <chrono>
#include <cmath>
#include <vector>

#include <conio.h>
#include <stdlib.h>

#include <thread>

#define FPS 60
#define VERTEX_ATTRIBUTE_SIZE 2

#define ANIMATION_SPEED 2.5e-3


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
		std::cout<<"Failed to compile "<< 
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
		std::cout<<message<<std::endl;
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
		std::cout<<"Не удалось создать вершинный шейдер"<<std::endl;
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


int main(int argc, char** argv)
{
	setlocale(LC_ALL, "Russian");

	std::cout << "Hello CMake." << std::endl;

	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();



	GLFWwindow* window;



	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		return -1;

	unsigned int buffer{};

	std::vector<float> vertexes = {
		0	,	1,
		-1	,	-1,
		0.99,	-1,
		0	,	1,
		1	,	1,
		1	,	-1
	};

	float bufferData[12];
	for(std::size_t i = 0; i!=vertexes.size();++i)
		bufferData[i] = vertexes[i];

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bufferData), bufferData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, VERTEX_ATTRIBUTE_SIZE, GL_FLOAT, GL_FALSE, sizeof(float) * VERTEX_ATTRIBUTE_SIZE, 0);

	std::string vertexShader =
R"(#version 330 core

layout(location=0) in vec4 position; 

void main()
{
	gl_Position = position;
}
	
)";
	std::string fragmentShader =
R"(#version 330 core

layout(location=0) out vec4 color; 

void main()
{
	color = vec4(1.0, 0.0, 0.0, 1.0);
}
	
)";
	GLuint shaderProgram = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shaderProgram);  
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		std::chrono::system_clock::duration durationSinceEpoch = now.time_since_epoch();
		std::chrono::milliseconds	millisecondsSinceEpoch =
		std::chrono::duration_cast<std::chrono::milliseconds>(durationSinceEpoch);
		std::chrono::seconds		secondsSinceEpoch =
		std::chrono::duration_cast<std::chrono::seconds>(millisecondsSinceEpoch);

		long diff = 2L;
		bufferData[0] = 
		-1. +
		(
			// Преобразование sin(x) в Ey э[0;1]
			(sin(millisecondsSinceEpoch.count() / (1./ ANIMATION_SPEED)) + 1.) / 2. *
			// Длина
			(bufferData[2*2] - vertexes[0])
		);

#if FPS <= 1000
		while (int(1000. / diff) >= FPS)
		{
#if FPS <= 100
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
#endif // FPS <= 100
			diff = std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now() - now).count();
		}
#endif  // FPS <= 1000
		{
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(bufferData), bufferData, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer
			(0, VERTEX_ATTRIBUTE_SIZE, GL_FLOAT, GL_FALSE, sizeof(float) * VERTEX_ATTRIBUTE_SIZE, 0);
		}
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);



		glDrawArrays(GL_TRIANGLES, NULL, sizeof(bufferData) / sizeof(*bufferData));

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		now = std::chrono::system_clock::now();
	}

	glDeleteProgram(shaderProgram);

	glfwTerminate();

	return 0;
}

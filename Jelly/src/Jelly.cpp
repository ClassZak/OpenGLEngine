#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>

#include <iostream>
#include <locale>
#include <chrono>
#include <cmath>
#include <vector>
#include <array>
#include <algorithm>

#ifdef _WIN32
#include <conio.h>
#endif
#include <stdlib.h>

#include <thread>
#include <utility>

#include <math.h>



#include <stb_image.h>



#include "utils/GLMacro.h"
#include "Vertex/Vertex2D.hpp"
#include "Vertex/VertexUtils.hpp"
#include "Shader/Shader.hpp"
#include "Shape/Circle.hpp"
#include "Shape/Line.hpp"




#define FPS 60
#define VERTEX_ATTRIBUTE_SIZE 2

#define ANIMATION_SPEED 2.5e-3



int main(int argc, char** argv)
{
	setlocale(LC_ALL, "Russian");

	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	std::string shaderData = LoadDataFromFile("../res/shaders/shader.shader");
	Shader shader = Shader::GetVertexAndFragmentShaders(shaderData);

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "Jelly", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback
	(
		window,
		[](GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}
	);
	GLFWimage images[1];
	images[0].pixels = stbi_load("../assets/icon.png", &images[0].width, &images[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);
#if FPS <= 1000
	glfwSwapInterval(1);
#endif

	if (glewInit() != GLEW_OK)
		return -1;


	const std::vector<Vertex2D<float>> vertexes =
	{
		Vertex2D(-0.5f		,	-0.5f),
		Vertex2D(-0.5f		,	0.5f),
		Vertex2D(0.5f		,	0.5f),

		Vertex2D(-0.5f		,	-0.5f),
		Vertex2D(0.5f		,	0.5f),
		Vertex2D(0.5f		,	-0.5f),
	};
	std::vector<Vertex2D<float>> vertexBufferData = GetUniqueVertexes(vertexes);
	std::vector<unsigned int> vertexesIndices =
	{
		0,	1,	2,
		0,	2,	3
	};

	unsigned int VAO1;
	GLLogCall(glGenVertexArrays(1, &VAO1));
	GLLogCall(glBindVertexArray(VAO1));

	unsigned int VBO1{};
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData
	(
		GL_ARRAY_BUFFER,
		vertexBufferData.size() * sizeof(std::vector<Vertex2D<float>>::value_type),
		vertexBufferData.data(),
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer
	(0, VERTEX_ATTRIBUTE_SIZE, GL_FLOAT, GL_FALSE, sizeof(float) * VERTEX_ATTRIBUTE_SIZE, 0);

	unsigned int IBO1{};
	glGenBuffers(1, &IBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO1);
	glBufferData
	(
		GL_ELEMENT_ARRAY_BUFFER,
		vertexesIndices.size() * sizeof(std::vector<unsigned int>::value_type),
		vertexesIndices.data(),
		GL_STATIC_DRAW
	);

	GLuint shaderProgram = Shader::CreateShader(shader.m_vertexShader, shader.m_fragmentShader);
	glUseProgram(shaderProgram);


	int location = glGetUniformLocation(shaderProgram, "u_Color");
	GL_ASSERT(location != -1);



	Circle circle
	(
		30,
		0.1f,
		Vertex2D<float>(0.75f,-0.5f),
		[&location](){glUniform4f(location, 0.2f, 0.2f, 02.f, 1.f);}
	);
	circle.Init();

	Line<float> line
	(
		{
			Vertex2D<float>(-0.5f, -0.5f),
			Vertex2D<float>(0.0f, 0.5f),
			Vertex2D<float>(0.5f, -0.5f),
			Vertex2D<float>(-1.f, -1.f),
		},
		GL_LINE_LOOP,
		[&]()
		{
			glUniform4f(location, 1.0f, 0.0f, 0.0f, 1.0f);
		}
	);
	line.Init();






	while (!glfwWindowShouldClose(window))
	{
		std::chrono::system_clock::duration durationSinceEpoch = now.time_since_epoch();
		std::chrono::milliseconds	millisecondsSinceEpoch =
		std::chrono::duration_cast<std::chrono::milliseconds>(durationSinceEpoch);
		std::chrono::seconds		secondsSinceEpoch =
		std::chrono::duration_cast<std::chrono::seconds>(durationSinceEpoch);


		vertexBufferData[1].x =
		vertexes[1].x + (cos(millisecondsSinceEpoch.count() * ANIMATION_SPEED) + 1) / 2.
		- 0.5;
		vertexBufferData[1].y =
		vertexes[1].y + (sin(millisecondsSinceEpoch.count() * ANIMATION_SPEED) + 1) / 2.
		- 0.5;


		long diff = 2L;
#if FPS <= 1000
		while (int(1000. / diff) >= FPS)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
			diff = std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now() - now).count();
		}
#endif  // FPS <= 1000

		glUniform4f
		(
			location,
			(sin(millisecondsSinceEpoch.count() * ANIMATION_SPEED) + 1) / 2.f,
			(cos(millisecondsSinceEpoch.count() * ANIMATION_SPEED) + 1) / 2.f,
			(tan(millisecondsSinceEpoch.count() * ANIMATION_SPEED) + 1),
			1.f
		);

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);




		glBindVertexArray(VAO1);
		glBindBuffer(GL_ARRAY_BUFFER, VBO1);
		glBufferData
		(
			GL_ARRAY_BUFFER,
			vertexBufferData.size() * sizeof(std::vector<Vertex2D<float>>::value_type),
			vertexBufferData.data(),
			GL_STATIC_DRAW
		);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO1);
		glBufferData
		(
			GL_ELEMENT_ARRAY_BUFFER,
			vertexesIndices.size() * sizeof(std::vector<unsigned int>::value_type) *
			vertexesIndices.size(),
			vertexesIndices.data(),
			GL_STATIC_DRAW
		);
		glDrawElements(GL_TRIANGLES, vertexesIndices.size(), GL_UNSIGNED_INT, nullptr);


		circle.Draw();
		line.Draw();


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		now = std::chrono::system_clock::now();
	}

	glDeleteProgram(shaderProgram);

	glDeleteVertexArrays(1, &VAO1);

	glfwTerminate();

	return 0;
}

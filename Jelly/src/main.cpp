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
		[&]()
		{
			glUniform4f(location, 1.0f, 0.0f, 0.0f, 1.0f);
		},
		GL_LINE_LOOP
	);
	line.Init();






	while (!glfwWindowShouldClose(window))
	{
		std::chrono::system_clock::duration durationSinceEpoch = now.time_since_epoch();
		std::chrono::milliseconds	millisecondsSinceEpoch =
		std::chrono::duration_cast<std::chrono::milliseconds>(durationSinceEpoch);
		std::chrono::seconds		secondsSinceEpoch =
		std::chrono::duration_cast<std::chrono::seconds>(durationSinceEpoch);

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




		circle.Draw();
		line.Draw();


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

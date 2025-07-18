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
#include <functional>
#include <utility>

#include <math.h>



#include <stb_image.h>



#include "utils/GLMacro.h"
#include "Vertex/Vertex2D.hpp"
#include "Vertex/VertexUtils.hpp"
#include "Shader/Shader.hpp"
#include "Shape/Circle.hpp"
#include "Shape/Line.hpp"
#include "Shape/Jelly.hpp"




#define FPS 60
#define VERTEX_ATTRIBUTE_SIZE 2

#define ANIMATION_SPEED 2.5e-3


std::vector<std::function<void(GLFWwindow* window, double xpos, double ypos)>> mouseMovingProcs;
float windowWidth	=640;
float windowHeight	=480;

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "Russian");

	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	std::string shader_data = LoadDataFromFile("../res/shaders/shader.shader");
	Shader shader = Shader::GetVertexAndFragmentShaders(shader_data);

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(windowWidth, windowHeight, "Jelly", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	/* Предустановленные Callback функции*/
	glfwSetFramebufferSizeCallback
	(
		window,
		[](GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
			windowHeight = height;
			windowWidth = width;
		}
	);
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)->void
		{
			for(std::size_t i=0;i!= mouseMovingProcs.size();++i)
				if(mouseMovingProcs[i])
					mouseMovingProcs[i](window, xpos, ypos);
		}
	);
	mouseMovingProcs.push_back([](GLFWwindow* window, double xpos, double ypos)->void
		{
			printf("(%.2f\t,\t%.2f)\n", xpos, ypos);
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

	GLuint shader_program = Shader::CreateShader(shader.m_vertexShader, shader.m_fragmentShader);
	glUseProgram(shader_program);


	int location = glGetUniformLocation(shader_program, "u_Color");
	GL_ASSERT(location != -1);





	Jelly jelly(shader_program);
	jelly.Init();
	mouseMovingProcs.push_back([&jelly](GLFWwindow* window, double xpos, double ypos)->void
		{
			jelly.Tugging(xpos / windowWidth, ypos/ windowHeight);
		}
	);




	while (!glfwWindowShouldClose(window))
	{
		std::chrono::system_clock::duration duration_since_epoch = now.time_since_epoch();
		std::chrono::milliseconds	milliseconds_since_epoch =
		std::chrono::duration_cast<std::chrono::milliseconds>(duration_since_epoch);
		std::chrono::seconds		seconds_since_epoch =
		std::chrono::duration_cast<std::chrono::seconds>(duration_since_epoch);

		long diff = 2L;
#if FPS <= 1000
		while (int(1000. / diff) >= FPS)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
			diff = std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now() - now).count();
		}
#endif  // FPS <= 1000

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(.7f,.7f,.7f,1.f);




		jelly.Draw();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		now = std::chrono::system_clock::now();
	}

	glDeleteProgram(shader_program);

	glfwTerminate();

	return 0;
}

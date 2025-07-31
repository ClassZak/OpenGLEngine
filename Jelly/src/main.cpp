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

#include "OpenGLClass/Shader.hpp"
#include "Shape/Circle.hpp"
#include "Shape/Line.hpp"
#include "Shape/Jelly.hpp"
#include "Shape/Quadrangle.hpp"
#include "Shape/CircleSector.hpp"
#include "Shape/SimpleRect.hpp"

#include "OpenGLClass/VertexBufferObject.hpp"
#include "OpenGLClass/IndexBufferObject.hpp"
#include "OpenGLClass/VertexArrayObject.hpp"
#include "OpenGLClass/Renderer.hpp"




#define FPS 60
#define VERTEX_ATTRIBUTE_SIZE 2

#define ANIMATION_SPEED 1.e-2


std::vector<std::function<void(GLFWwindow* window, double xpos, double ypos)>> mouseMovingProcs;
float windowWidth	=640;
float windowHeight	=480;

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "Russian");
	srand(time(NULL));

	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

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

	
	
	GLFWimage images[1];
	images[0].pixels = stbi_load("../assets/icon.png", &images[0].width, &images[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);
#if FPS <= 1000
	glfwSwapInterval(1);
#endif

	if (glewInit() != GLEW_OK)
		return -1;
	std::cout<<glGetString(GL_VERSION)<<std::endl;



	
	std::vector<Vertex2D<float>> vertexes =
	{
		Vertex2D(-0.5f	,	0.5f),
		Vertex2D(0.5f	,	0.5f),
		Vertex2D(0.5f	,	-0.5f),
		Vertex2D(-0.5f	,	-0.5f),
	};
	VertexArrayObject vertexArrayObject;
	VertexBufferObject vertexBuffer(vertexes);
	VertexBufferLayout layout;
	layout.Push<float>(2);
	vertexArrayObject.AddBuffer(vertexBuffer, layout);
	IndexBufferObject indexBufferObject({ 0, 1, 2, 0, 2, 3 });
	Shader shader = Shader("../res/shaders/shader.shader");
	shader.Bind();
	shader.SetUniform_4f("u_Color", {1.f, 0.f, 0.f, 1.f});

	vertexArrayObject.UnBind();
	vertexBuffer.UnBind();
	indexBufferObject.UnBind();
	shader.UnBind();

	SimpleRect rect;


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

		Renderer::GetInstance().
		Draw
		(
			(IHasVertexBufferObject*)&rect,
			"u_Color",
			{ (milliseconds_since_epoch.count() % 1000) / 1000.f, 0.f, 0.f, 1.f}
		);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();


		now = std::chrono::system_clock::now();
	}

	glfwTerminate();

	return 0;
}

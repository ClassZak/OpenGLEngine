#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

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
#include "Shape/OldCircle.hpp"
#include "Shape/Circle.hpp"
#include "Shape/OldLine.hpp"
#include "Shape/Jelly.hpp"
#include "Shape/OldQuadrangle.hpp"
#include "Shape/Quadrangle.hpp"
#include "Shape/CircleSector.hpp"
#include "Shape/SimpleRect.hpp"

#include "OpenGLClass/VertexBufferObject.hpp"
#include "OpenGLClass/IndexBufferObject.hpp"
#include "OpenGLClass/VertexArrayObject.hpp"
#include "OpenGLClass/Renderer.hpp"




#define FPS 60
#define VERTEX_ATTRIBUTE_SIZE 2

#define ANIMATION_SPEED 1.e-3


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


	Renderer::GetInstance().AddShader("../res/shaders/shader.shader");


	Line line({Vertex2D(-1.f,-1.f), Vertex2D(0.5f, 1.f)});
	SimpleRect rect(Vertex2D(-0.9f,-0.9f),0.5f, 0.5f);
	CircleSector circleSector(30, 0.2f, Vertex2D<float>(0.f,0.f), 0, M_PI_2);
	Quadrangle quadrangle({Vertex2D(1.f,.9f), Vertex2D(-.4f,.9f), Vertex2D(0.2f, 0.6f), Vertex2D(0.2f, 0.3f)});
	auto& rendererShader = Renderer::GetInstance().FindShader(3);
	rect.SetShader(rendererShader);
	line.SetShader(rendererShader);
	circleSector.SetShader(rendererShader);
	quadrangle.SetShader(rendererShader);



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



		/* Animate here */
		quadrangle.GetVertices()[0].x = sin(milliseconds_since_epoch.count() * ANIMATION_SPEED / 10);
		quadrangle.ReBind();
		rect.SetHeight(sin(milliseconds_since_epoch.count() * ANIMATION_SPEED));

		line.GetVertices()[0].x = cos(milliseconds_since_epoch.count() * ANIMATION_SPEED);
		line.GetVertices()[0].y = sin(milliseconds_since_epoch.count() * ANIMATION_SPEED);
		line.GetVertices()[1].x = -cos(milliseconds_since_epoch.count() * ANIMATION_SPEED);
		line.GetVertices()[1].y = -sin(milliseconds_since_epoch.count() * ANIMATION_SPEED);
		line.ReBind();

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(.7f,.7f,.7f,1.f);

		
		Renderer::GetInstance()
		/*.Draw
		(
			&rect,
			Uniform
			(
				"u_Color",
				UniformVec4{(milliseconds_since_epoch.count() % 1000) / 1000.f, 0.f, 0.f, 1.f}
			)
		)
		.Draw(&circleSector)*/
		.Draw
		(
			&line,
			Uniform
			(
				"u_Color",
				UniformVec4{ 0.f, (milliseconds_since_epoch.count() % 1000) / 1000.f, 0.f, 1.f }
			)
		)
		/*.Draw
		(
			&quadrangle,
			Uniform
			(
				"u_Color",
				UniformVec4{ 0.f, (milliseconds_since_epoch.count() % 100) / 100.f, 0.f, 1.f }
			)
		)*/;



		

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();


		now = std::chrono::system_clock::now();
	}

	glfwTerminate();

	return 0;
}

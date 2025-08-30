#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION
#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <tiny_obj_loader.h>

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
#include "Vertex/Vertex3DText.hpp"
#include "Vertex/Vertex2DText.hpp"
#include "Vertex/VertexUtils.hpp"

#include "OpenGLClass/Shader.hpp"
#include "Shape/Circle.hpp"
#include "Shape/Jelly.hpp"
#include "Shape/JellyWithTexture.hpp"
#include "Shape/Quadrangle.hpp"
#include "Shape/CircleSector.hpp"
#include "Shape/SimpleRect.hpp"

#include "OpenGLClass/VertexBufferObject.hpp"
#include "OpenGLClass/IndexBufferObject.hpp"
#include "OpenGLClass/VertexArrayObject.hpp"

#include "OpenGLClass/Shader.hpp"
#include "OpenGLClass/Texture.hpp"
#include "OpenGLClass/Mesh.hpp"

#include "EngineCore/Renderer.hpp"
#include "EngineCore/AssetsManager.hpp"



#define FPS 60
#define VERTEX_ATTRIBUTE_SIZE 2

#define ANIMATION_SPEED 1.e-3


std::vector<std::function<void(GLFWwindow* window, double xpos, double ypos)>> mouseMovingProcs;
float windowWidth	=640;
float windowHeight	=480;


/*Вид и проекция*/
glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::mat4 model = glm::mat4(1.f);
glm::mat4 view = glm::lookAt
(
	camera_pos, // позиция камеры
	glm::vec3(0.0f, 0.0f, 0.0f), // цель камеры
	glm::vec3(0.0f, 0.0f, 1.0f)  // вектор "вверх"
);
glm::mat4 projection = glm::perspective
(
	glm::radians(15.f),							// угол обзора
	(float)windowWidth / (float)windowHeight,	// соотношение сторон
	0.1f,										// ближняя плоскость отсечения
	100.0f										// дальняя плоскость отсечения
);


enum CameraDirection
{
	None = -1,
	Forward,
	Backward,
	Left,
	Right,
	Up,
	Down
};

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

	AssetsManager::GetInstance().LoadShader("default_shader", "../res/shaders/shader.shader");
	auto texture_shader = 
	AssetsManager::GetInstance().LoadShader("texture_shader", "../res/shaders/texture_shader.shader");
	auto model_shader = 
	AssetsManager::GetInstance().LoadShader("model_shader", "../res/shaders/model_shader.shader");

	auto jelly_texture =
	AssetsManager::GetInstance().LoadTexture("jelly_texture", "../assets/jelly_texture.png");
	auto cube_texture =
	AssetsManager::GetInstance().LoadTexture("cube_texture", "../assets/models/cube/default.png");

	JellyWithTexture jelly(jelly_texture, texture_shader);

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
		jelly.Animate(milliseconds_since_epoch.count(), ANIMATION_SPEED);

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(.7f,.7f,.7f,1.f);


		/*texture_shader->Bind();
		texture_shader->SetUniform
		({ "u_Color", UniformVec4(1.f, 1.f, 1.f, 1.f) });*/
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		
		Renderer::GetInstance().Draw(&jelly, { "u_Color", UniformVec4(1.f, 1.f, 1.f, 1.f) });

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();


		now = std::chrono::system_clock::now();
	}

	glfwTerminate();

	return 0;
}
